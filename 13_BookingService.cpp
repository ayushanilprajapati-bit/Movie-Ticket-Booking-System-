// 13_BookingService.cpp
// Responsibility: the orchestrator. Runs the booking flow end to end by calling the
// other classes — it does NOT do their jobs itself (no pricing math, no payment
// handling, no printing logic lives here; those are delegated).
// Relationships: BookingService *-- Booking (owns the master list of booking records),
// BookingService --> Cinema (uses the catalogue), and "uses" PriceCalculator/TicketPrinter.
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <functional>
#include <map>
using namespace std;

class BookingService {
private:
    Cinema& cinema;              // association: uses, does not own, the Cinema
    vector<Booking> bookings;    // composition: BookingService owns booking records
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;

    // Dependency Inversion: BookingService depends only on the Payment ABSTRACTION
    // and on a std::function factory type — it never names UpiPayment, CardPayment,
    // CashPayment, or any future NetBankingPayment anywhere in this file. The
    // concrete classes are wired up from outside via registerPaymentMethod()
    // (see main.cpp), which is also what makes this Open/Closed: adding a brand
    // new payment method later needs ONE new class + ONE registration call in
    // main.cpp — zero edits inside BookingService.
    map<string, function<unique_ptr<Payment>()>> paymentFactories;

    unique_ptr<Payment> createPaymentFor(const string& method) {
        auto it = paymentFactories.find(method);
        if (it == paymentFactories.end()) return nullptr;
        return it->second();
    }

public:
    BookingService(Cinema& cinema) : cinema(cinema) {
        // Reserved up front: bookings.push_back() can reallocate the vector, which
        // would invalidate any Booking* / Booking& a caller kept from an earlier
        // call. Reserving capacity keeps this demo safe; see Step G for the fuller
        // discussion of what a production version would do differently here.
        bookings.reserve(200);
    }

    // Open/Closed + Dependency Inversion in action: the composition root (main.cpp)
    // calls this once per supported payment method. BookingService's own code
    // never changes when a new method is added.
    void registerPaymentMethod(const string& name, function<unique_ptr<Payment>()> factory) {
        paymentFactories[name] = factory;
    }

    // F4 + F5 + F6 + F7: validate seats, price them, take payment, print ticket.
    // Returns a pointer to the stored Booking (CONFIRMED or FAILED) or nullptr if the
    // seat selection itself was invalid (bad seat number / already booked).
    Booking* bookSeats(Show* show, const vector<string>& seatNumbers,
                        Customer* customer, const string& paymentMethod) {
        if (show == nullptr || seatNumbers.empty()) {
            cout << "  Invalid show or no seats selected.\n";
            return nullptr;
        }

        // Step 1: validate EVERY seat before touching any state (F4 — "if any
        // selected seat is already BOOKED, the whole booking is rejected and no
        // seat changes state").
        vector<ShowSeat*> selected;
        for (const string& num : seatNumbers) {
            ShowSeat* ss = show->findShowSeat(num);
            if (ss == nullptr) {
                cout << "  Seat " << num << " does not exist on this show. Booking cancelled, no changes made.\n";
                return nullptr;
            }
            if (!ss->isAvailable()) {
                cout << "  Seat " << num << " is already BOOKED. Booking cancelled, no changes made.\n";
                return nullptr;
            }
            selected.push_back(ss);
        }

        // Step 2: all seats are valid & free -> book them together.
        for (ShowSeat* ss : selected) ss->book();

        // Step 3: price it (delegated to PriceCalculator — single source of truth).
        double total = priceCalculator.calculateTotal(selected);

        // Step 4: create the Booking record («create» Booking).
        bookings.push_back(Booking(show, customer, selected, total));
        Booking& booking = bookings.back();

        printPriceBreakdown(selected, total);

        // Step 5: pay («create» the right Payment subclass, then call pay()).
        unique_ptr<Payment> payment = createPaymentFor(paymentMethod);
        if (payment == nullptr) {
            cout << "  Unknown payment method.\n";
            for (ShowSeat* ss : selected) ss->release();
            booking.markFailed();
            return &booking;
        }

        bool paid = payment->pay(total);   // runtime polymorphism: real pay() picked at run time
        booking.attachPayment(move(payment));   // keep the payment record either way (audit trail)

        // F6: a failed payment must NOT confirm the booking; seats are released.
        if (!paid) {
            for (ShowSeat* ss : selected) ss->release();
            booking.markFailed();
            cout << "  Payment failed — booking NOT confirmed, seats released back to AVAILABLE.\n";
            return &booking;
        }

        booking.confirm();
        ticketPrinter.printTicket(booking);
        return &booking;
    }

    // F8: cancel a booking -> its seats become AVAILABLE again.
    bool cancelBooking(const string& bookingId) {
        for (Booking& b : bookings) {
            if (b.getBookingId() == bookingId) {
                if (b.cancel()) {
                    cout << "  Booking " << bookingId << " cancelled. Seats are AVAILABLE again.\n";
                    return true;
                }
                cout << "  Booking " << bookingId << " cannot be cancelled (status: "
                     << b.getStatusString() << ").\n";
                return false;
            }
        }
        cout << "  No booking found with id " << bookingId << ".\n";
        return false;
    }

    void listMyTickets(const string& customerPhone) const {
        bool any = false;
        for (const Booking& b : bookings) {
            if (b.getCustomer() != nullptr && b.getCustomer()->getPhone() == customerPhone) {
                cout << "  " << b.getBookingId() << "  " << b.getShow()->getMovie()->getTitle()
                     << "  Rs." << b.getTotalAmount() << "  [" << b.getStatusString() << "]\n";
                any = true;
            }
        }
        if (!any) cout << "  No bookings found for this phone number.\n";
    }

private:
    void printPriceBreakdown(const vector<ShowSeat*>& seats, double total) const {
        cout << "\n";
        for (ShowSeat* s : seats) {
            cout << "  " << s->getSeat()->getSeatNumber() << " "
                 << seatTypeToString(s->getSeat()->getType()) << " Rs."
                 << PriceCalculator::getPrice(s->getSeat()->getType()) << "\n";
        }
        cout << "  TOTAL          Rs." << total << "\n";
    }
};

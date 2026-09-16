// 08_Booking.cpp
// Responsibility: booking id, which show, which seats, total amount, status.
// Relationships:
//   Booking --> Customer  (ASSOCIATION) — references a Customer it does not own.
//   Booking o-- ShowSeat  (AGGREGATION) — references existing ShowSeats (owned by Show);
//                          they keep existing even if this Booking is cancelled/destroyed.
//   Booking *-- Payment   (COMPOSITION) — the Payment record is created specifically for,
//                          and only meaningful within, this one Booking; it is not shared
//                          and is destroyed with the Booking.
// Must NOT: calculate price, talk to a payment gateway, or print itself — those are
// PriceCalculator's, Payment's and TicketPrinter's jobs respectively (Single Responsibility).
#include <string>
#include <vector>
#include <memory>
using namespace std;

enum class BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

class Booking {
private:
    string bookingId;
    Show* show;                      // association
    Customer* customer;               // association
    vector<ShowSeat*> bookedSeats;    // aggregation
    unique_ptr<Payment> payment;      // composition (owned by this Booking).
                                       // unique_ptr also makes Booking move-only,
                                       // which is exactly what a vector<Booking> needs
                                       // when it reallocates — no accidental shallow
                                       // copy / double-delete of the owned Payment.
    double totalAmount;
    BookingStatus status;

    static int nextId;   // static member -> shared by ALL Booking objects

public:
    Booking() : show(nullptr), customer(nullptr),
                totalAmount(0.0), status(BookingStatus::PENDING) {}

    Booking(Show* show, Customer* customer, vector<ShowSeat*> seats, double totalAmount) {
        this->bookingId = "BK" + to_string(1000 + nextId);
        nextId++;
        this->show = show;
        this->customer = customer;
        this->bookedSeats = seats;
        this->totalAmount = totalAmount;
        this->status = BookingStatus::PENDING;
    }

    void attachPayment(unique_ptr<Payment> p) { payment = move(p); }

    void confirm() { status = BookingStatus::CONFIRMED; }
    void markFailed() { status = BookingStatus::FAILED; }

    // F8: cancel a CONFIRMED booking -> its seats become AVAILABLE again
    bool cancel() {
        if (status != BookingStatus::CONFIRMED) return false;
        for (ShowSeat* s : bookedSeats) {
            s->release();
        }
        status = BookingStatus::CANCELLED;
        return true;
    }

    string getBookingId() const { return bookingId; }
    Show* getShow() const { return show; }
    Customer* getCustomer() const { return customer; }
    const vector<ShowSeat*>& getBookedSeats() const { return bookedSeats; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }

    string getStatusString() const {
        switch (status) {
            case BookingStatus::PENDING:   return "PENDING";
            case BookingStatus::CONFIRMED: return "CONFIRMED";
            case BookingStatus::FAILED:    return "FAILED";
            case BookingStatus::CANCELLED: return "CANCELLED";
        }
        return "UNKNOWN";
    }
};

int Booking::nextId = 1;

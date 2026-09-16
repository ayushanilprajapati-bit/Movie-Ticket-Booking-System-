// 12_TicketPrinter.cpp
// Responsibility: format and print a ticket. Printing only.
// Must NOT: mutate the booking, calculate price, or take payment — it only reads
// data that already exists on the Booking and displays it (F7).
#include <iostream>
using namespace std;

class TicketPrinter {
public:
    void printTicket(const Booking& booking) const {
        cout << "\n  ================ TICKET ================\n";
        cout << "  Booking ID : " << booking.getBookingId() << "\n";
        cout << "  Movie      : " << booking.getShow()->getMovie()->getTitle() << "\n";
        cout << "  Screen     : Screen-" << booking.getShow()->getScreen()->getScreenNumber()
             << "   " << booking.getShow()->getStartTime() << "\n";
        cout << "  Seats      : ";
        const vector<ShowSeat*>& seats = booking.getBookedSeats();
        for (size_t i = 0; i < seats.size(); i++) {
            cout << seats[i]->getSeat()->getSeatNumber();
            if (i + 1 < seats.size()) cout << ", ";
        }
        cout << "\n";
        cout << "  Amount     : Rs." << booking.getTotalAmount()
             << "        Status: " << booking.getStatusString() << "\n";
        cout << "  =========================================\n";
    }
};

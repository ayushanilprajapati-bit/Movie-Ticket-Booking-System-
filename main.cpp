
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <memory>
#include <functional>
using namespace std;

#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"     
#include "05_Show.cpp"         
#include "04_Cinema.cpp"      
#include "07_Customer.cpp"
#include "09_Payment.cpp"      
#include "10_PaymentTypes.cpp" 
#include "11_PriceCalculator.cpp" 
#include "08_Booking.cpp"      
#include "12_TicketPrinter.cpp" 
#include "13_BookingService.cpp"

const int EOF_CHOICE = -999;   
int readIntChoice() {
    string line;
    if (!getline(cin, line)) return EOF_CHOICE;
    stringstream ss(line);
    int value;
    if (!(ss >> value)) return -1;  
    return value;
}

vector<string> splitSeats(const string& csv) {
    vector<string> result;
    stringstream ss(csv);
    string token;
    while (getline(ss, token, ',')) {
       
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        if (start != string::npos) result.push_back(token.substr(start, end - start + 1));
    }
    return result;
}


class CinemaApp {
private:
    Cinema& cinema;
    BookingService bookingService;
    Customer customer;

public:
    CinemaApp(Cinema& cinema, Customer customer)
        : cinema(cinema), bookingService(cinema), customer(customer) {
        bookingService.registerPaymentMethod("UPI",  [] { return make_unique<UpiPayment>(); });
        bookingService.registerPaymentMethod("CARD", [] { return make_unique<CardPayment>(); });
        bookingService.registerPaymentMethod("CASH", [] { return make_unique<CashPayment>(); });
    }

    void run() {
        while (true) {
            cout << "\n===== MOVIE TICKET BOOKING =====\n";
            cout << "1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\n";
            cout << "Choose: ";
            int choice = readIntChoice();

            if (choice == 1) {
                handleListMovies();
            } else if (choice == 2) {
                handleBook();
            } else if (choice == 3) {
                handleCancel();
            } else if (choice == 4) {
                handleMyTickets();
            } else if (choice == 0 || choice == EOF_CHOICE) {
                cout << "\nThank you for using MOVIE TICKET BOOKING. Goodbye!\n";
                break;
            } else {
                // Edge case: invalid menu choice -> clear message, no crash
                cout << "  Invalid choice. Please choose one of the listed options.\n";
            }
        }
    }

private:
    void handleListMovies() {
        cinema.listMovies();
    }

    void handleBook() {
        if (cinema.getMovies().empty()) {
            cout << "  No movies available right now.\n";
            return;
        }
        cinema.listMovies();
        cout << "Choose movie: ";
        int movieChoice = readIntChoice();
        if (movieChoice < 1 || movieChoice > (int)cinema.getMovies().size()) {
            cout << "  Invalid movie choice.\n";
            return;
        }
        string movieTitle = cinema.getMovies()[movieChoice - 1].getTitle();

        vector<Show*> shows = cinema.getShowsForMovie(movieTitle);
        if (shows.empty()) {
            cout << "  No shows scheduled for this movie.\n";
            return;
        }
        cout << "\n";
        for (size_t i = 0; i < shows.size(); i++) {
            cout << "  [" << (i + 1) << "] Screen-" << shows[i]->getScreen()->getScreenNumber()
                 << "   " << shows[i]->getStartTime() << "\n";
        }
        cout << "Choose show: ";
        int showChoice = readIntChoice();
        if (showChoice < 1 || showChoice > (int)shows.size()) {
            cout << "  Invalid show choice.\n";
            return;
        }
        Show* show = shows[showChoice - 1];

        show->printSeatLayout();
        cout << "\nSeats (e.g. A1,B2): ";
        string seatLine;
        getline(cin, seatLine);
        vector<string> seatNumbers = splitSeats(seatLine);
        if (seatNumbers.empty()) {
            cout << "  No seats entered.\n";
            return;
        }

        cout << "\nPay by: 1.UPI  2.Card  3.Cash > ";
        int payChoice = readIntChoice();
        string method;
        if (payChoice == 1) method = "UPI";
        else if (payChoice == 2) method = "CARD";
        else if (payChoice == 3) method = "CASH";
        else {
            cout << "  Invalid payment method.\n";
            return;
        }

        bookingService.bookSeats(show, seatNumbers, &customer, method);
    }

    void handleCancel() {
        cout << "Enter booking id to cancel (e.g. BK1001): ";
        string id;
        getline(cin, id);
        if (id.empty()) {
            cout << "  No booking id entered.\n";
            return;
        }
        bookingService.cancelBooking(id);
    }

    void handleMyTickets() {
        bookingService.listMyTickets(customer.getPhone());
    }
};

int main() {
    Cinema cinema("PVR Cineplex");

    cinema.addMovie(Movie("3 Idiots", "Hindi", 170));
    cinema.addMovie(Movie("Interstellar", "English", 169));

    cinema.addScreen(Screen(1, 4, 3, 2));   
    cinema.addScreen(Screen(2, 4, 3, 2));  

    cinema.addShow("3 Idiots", 1, "06:00 PM");
    cinema.addShow("3 Idiots", 2, "09:00 PM");
    cinema.addShow("Interstellar", 1, "09:30 PM");

    cout << "Enter your name: ";
    string name;
    getline(cin, name);
    cout << "Enter your phone: ";
    string phone;
    getline(cin, phone);

    CinemaApp app(cinema, Customer(name, phone));
    app.run();
    return 0;
}

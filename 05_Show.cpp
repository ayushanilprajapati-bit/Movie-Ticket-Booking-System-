// 05_Show.cpp
// Responsibility: ONE screening = a Movie on a Screen at a start time; owns its ShowSeats.
// Relationships:
//   Show o-- Movie   (AGGREGATION) — Show borrows a Movie. Cancel/destroy a Show and the
//                      Movie still exists and can still be shown elsewhere.
//   Show --> Screen  (ASSOCIATION) — Show references an existing Screen but does not own
//                      it; the same physical Screen keeps hosting other shows at other times.
//   Show *-- ShowSeat (COMPOSITION) — Show builds one ShowSeat per physical seat, and those
//                      ShowSeats mean nothing outside this specific show; destroy the Show
//                      and its ShowSeats go with it.
// Must NOT: handle payment, pricing, or booking creation — only exposes seat state.
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class Show {
private:
    int showId;
    const Movie* movie;      // aggregation
    const Screen* screen;    // association
    string startTime;
    vector<ShowSeat> showSeats;  // composition

public:
    Show() : showId(0), movie(nullptr), screen(nullptr) {}

    Show(int showId, const Movie* movie, const Screen* screen, string startTime) {
        this->showId = showId;
        this->movie = movie;
        this->screen = screen;
        this->startTime = startTime;
        // Show builds its own ShowSeats, one per physical seat on the screen
        for (const Seat& s : screen->getSeats()) {
            showSeats.push_back(ShowSeat(&s));
        }
    }

    int getShowId() const { return showId; }
    const Movie* getMovie() const { return movie; }
    const Screen* getScreen() const { return screen; }
    string getStartTime() const { return startTime; }

    vector<ShowSeat>& getShowSeats() { return showSeats; }

    ShowSeat* findShowSeat(const string& seatNumber) {
        for (ShowSeat& ss : showSeats) {
            if (ss.getSeat()->getSeatNumber() == seatNumber) return &ss;
        }
        return nullptr;
    }

    // F3: display the seat layout with AVAILABLE / BOOKED status, grouped by type
    void printSeatLayout() const {
        cout << "\n  SCREEN-" << screen->getScreenNumber() << "  " << startTime
             << "  |  " << movie->getTitle() << "\n";
        printRow("SILVER", SeatType::SILVER);
        printRow("GOLD", SeatType::GOLD);
        printRow("PLATINUM", SeatType::PLATINUM);
        cout << "  ( [ ] = available   [X] = booked )\n";
    }

private:
    void printRow(const string& label, SeatType type) const {
        cout << "  " << left << setw(9) << label;
        for (const ShowSeat& ss : showSeats) {
            if (ss.getSeat()->getType() == type) {
                cout << ss.getSeat()->getSeatNumber()
                     << (ss.isAvailable() ? "[ ] " : "[X] ");
            }
        }
        cout << "\n";
    }
};

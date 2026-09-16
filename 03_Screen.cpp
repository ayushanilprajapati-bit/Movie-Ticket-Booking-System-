// 03_Screen.cpp
// Responsibility: represent ONE auditorium — a screen number and the seats it owns.
// Relationship: Screen *-- Seat (COMPOSITION). Screen builds its own seats in its
// constructor; a Seat has no meaning outside the Screen that created it, so if the
// Screen object is destroyed, its Seats are destroyed with it (lifetime test).
// Must NOT: know about movies, shows, or booking status.
#include <string>
#include <vector>
using namespace std;

class Screen {
private:
    int screenNumber;
    vector<Seat> seats;   // composition: Screen creates & owns these

public:
    Screen() : screenNumber(0) {}

    // Builds a small fixed layout: row A = SILVER, row B = GOLD, row C = PLATINUM
    Screen(int screenNumber, int silverCount, int goldCount, int platinumCount) {
        this->screenNumber = screenNumber;
        for (int i = 1; i <= silverCount; i++) {
            seats.push_back(Seat("A" + to_string(i), SeatType::SILVER));
        }
        for (int i = 1; i <= goldCount; i++) {
            seats.push_back(Seat("B" + to_string(i), SeatType::GOLD));
        }
        for (int i = 1; i <= platinumCount; i++) {
            seats.push_back(Seat("C" + to_string(i), SeatType::PLATINUM));
        }
    }

    int getScreenNumber() const { return screenNumber; }
    const vector<Seat>& getSeats() const { return seats; }
};

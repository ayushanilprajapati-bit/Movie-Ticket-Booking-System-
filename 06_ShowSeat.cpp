// 06_ShowSeat.cpp
// Responsibility: the AVAILABLE/BOOKED status of ONE physical seat FOR ONE show.
// Why this class exists and not just Seat: Seat A1 exists once on the Screen, but its
// status is different for every show (booked for 6 PM, free for 9 PM). Status belongs
// to the show, not to the physical chair.
// Relationship: ShowSeat --o Seat (AGGREGATION). ShowSeat only stores a pointer to an
// existing Seat that lives on the Screen; the Seat is not created by ShowSeat and keeps
// existing (on the Screen) even after this ShowSeat, or the whole Show, is destroyed.
// Must NOT: calculate price, and must NOT talk to Payment directly.
#include <stdexcept>
using namespace std;

enum class SeatStatus { AVAILABLE, BOOKED };

class ShowSeat {
private:
    const Seat* seat;   // aggregation: does not own the Seat
    SeatStatus status;

public:
    ShowSeat() : seat(nullptr), status(SeatStatus::AVAILABLE) {}

    ShowSeat(const Seat* seat) {
        this->seat = seat;
        this->status = SeatStatus::AVAILABLE;
    }

    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

    // Encapsulation: status can only change through these validated methods,
    // never by direct assignment from outside.
    void book() {
        if (status == SeatStatus::BOOKED) {
            throw runtime_error("Seat " + seat->getSeatNumber() + " is already booked");
        }
        status = SeatStatus::BOOKED;
    }

    void release() {
        status = SeatStatus::AVAILABLE;
    }

    const Seat* getSeat() const { return seat; }
    SeatStatus getStatus() const { return status; }
};

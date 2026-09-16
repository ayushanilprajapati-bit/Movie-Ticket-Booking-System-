// 02_Seat.cpp
// Responsibility: describe ONE physical seat (number + type).
// Must NOT: store booking status (that belongs to ShowSeat, not the physical chair)
//           and must NOT store price (that is PriceCalculator's single source of truth).
#include <string>
using namespace std;

enum class SeatType { SILVER, GOLD, PLATINUM };

inline string seatTypeToString(SeatType t) {
    switch (t) {
        case SeatType::SILVER:   return "SILVER";
        case SeatType::GOLD:     return "GOLD";
        case SeatType::PLATINUM: return "PLATINUM";
    }
    return "UNKNOWN";
}

class Seat {
private:
    string seatNumber;   // e.g. "A1"
    SeatType type;

public:
    Seat() : seatNumber(""), type(SeatType::SILVER) {}

    Seat(string seatNumber, SeatType type) {
        this->seatNumber = seatNumber;
        this->type = type;
    }

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
};

// 11_PriceCalculator.cpp
// Responsibility: turn a list of seats into a total amount. This is the ONE place
// prices live (F5) — no other class stores or hardcodes a seat price.
// Must NOT: create bookings, handle payment, or print tickets.
#include <vector>
using namespace std;

class PriceCalculator {
public:
    // Constants instead of magic numbers.
    static constexpr double SILVER_PRICE = 150.0;
    static constexpr double GOLD_PRICE = 250.0;
    static constexpr double PLATINUM_PRICE = 400.0;

    static double getPrice(SeatType type) {
        switch (type) {
            case SeatType::SILVER:   return SILVER_PRICE;
            case SeatType::GOLD:     return GOLD_PRICE;
            case SeatType::PLATINUM: return PLATINUM_PRICE;
        }
        return 0.0;
    }

    static double calculateTotal(const vector<ShowSeat*>& seats) {
        double total = 0.0;
        for (ShowSeat* s : seats) {
            total += getPrice(s->getSeat()->getType());
        }
        return total;
    }
};

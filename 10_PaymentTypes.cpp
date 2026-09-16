// 10_PaymentTypes.cpp
// Inheritance: each class IS-A Payment and provides its own pay() implementation.
// This is what lets BookingService call payment->pay(total) through a Payment*
// and get runtime polymorphism (F6): the correct pay() runs depending on the
// object's real type, decided at run time, not at compile time.
#include <iostream>
using namespace std;

class UpiPayment : public Payment {
public:
    bool pay(double amount) override {
        this->amount = amount;
        // Simulated gateway: UPI is treated as always reachable for this demo.
        cout << "  [UPI] Rs." << amount << " paid successfully\n";
        return true;
    }
};

class CardPayment : public Payment {
private:
    // Simulated business rule for demo/testing purposes only (F6 edge case:
    // "a failed payment must NOT confirm the booking"). Not a real card network limit.
    static const int CARD_LIMIT = 1000;

public:
    bool pay(double amount) override {
        this->amount = amount;
        if (amount > CARD_LIMIT) {
            cout << "  [CARD] Payment DECLINED (amount Rs." << amount
                 << " exceeds simulated card limit Rs." << CARD_LIMIT << ")\n";
            return false;
        }
        cout << "  [CARD] Rs." << amount << " paid successfully\n";
        return true;
    }
};

class CashPayment : public Payment {
public:
    bool pay(double amount) override {
        this->amount = amount;
        cout << "  [CASH] Rs." << amount << " received successfully\n";
        return true;
    }
};

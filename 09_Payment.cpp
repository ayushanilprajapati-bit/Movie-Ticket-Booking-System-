// 09_Payment.cpp
// Abstraction: Payment defines ONLY the payment contract. It does not know HOW a
// payment is actually carried out — that is left entirely to its subclasses.
// Must NOT: know about Booking, Show, or seats — it only knows an amount.
class Payment {
protected:
    double amount;

public:
    Payment() : amount(0.0) {}
    virtual ~Payment() {}   // virtual destructor: safe to delete through a Payment*

    // Pure virtual -> Payment is abstract; runtime polymorphism happens through
    // this method when called via a Payment* / Payment& (see BookingService).
    virtual bool pay(double amount) = 0;
};

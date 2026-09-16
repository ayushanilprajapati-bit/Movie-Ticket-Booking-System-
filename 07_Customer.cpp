// 07_Customer.cpp
// Responsibility: a customer's own identity (name, phone). Nothing else.
// Relationship: Booking --> Customer (ASSOCIATION). A Customer exists independently of
// any single booking, before and after it; destroying a Booking never destroys the
// Customer, and one Customer can have many Bookings.
#include <string>
using namespace std;

class Customer {
private:
    string name;
    string phone;

public:
    Customer() : name(""), phone("") {}

    Customer(string name, string phone) {
        this->name = name;
        this->phone = phone;
    }

    string getName() const { return name; }
    string getPhone() const { return phone; }
};

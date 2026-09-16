// 01_Movie.cpp
// Responsibility: hold a movie's own data (title, language, duration).
// Must NOT: know about shows, screens, seats, or bookings.
#include <string>
using namespace std;

class Movie {
private:
    string title;
    string language;
    int durationMinutes;

public:
    // Overloaded constructors -> compile-time polymorphism
    Movie() : title("Untitled"), language("Unknown"), durationMinutes(0) {}

    Movie(string title, string language, int durationMinutes) {
        this->title = title;                 // 'this' disambiguates member vs parameter
        this->language = language;
        this->durationMinutes = durationMinutes;
    }

    string getTitle() const { return title; }
    string getLanguage() const { return language; }
    int getDuration() const { return durationMinutes; }
};

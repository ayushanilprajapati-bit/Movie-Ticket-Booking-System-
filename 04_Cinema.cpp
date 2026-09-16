// 04_Cinema.cpp
// Responsibility: the theatre itself — its name, and the inventory of screens, movies
// and shows it hosts.
// Relationship: Cinema *-- Screen (COMPOSITION). Cinema creates and owns its Screens;
// a Screen has no independent existence outside a Cinema — destroy the Cinema and its
// Screens (and, transitively, their Seats) are destroyed too.
// Must NOT: handle payment, pricing, ticket printing, or booking logic — those are
// BookingService's job. Cinema only manages the catalogue.
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class Cinema {
private:
    string name;
    vector<Screen> screens;   // composition
    vector<Movie> movies;
    vector<Show> shows;
    int nextShowId = 1;

public:
    Cinema() {}
    Cinema(string name) : name(name) {}

    void addScreen(const Screen& screen) { screens.push_back(screen); }
    void addMovie(const Movie& movie) { movies.push_back(movie); }

    // Cinema orchestrates creation of Shows (it owns the movie & screen catalogue),
    // but this is still just catalogue management, not booking logic.
    void addShow(const string& movieTitle, int screenNumber, const string& startTime) {
        const Movie* moviePtr = findMovieByTitle(movieTitle);
        const Screen* screenPtr = findScreenByNumber(screenNumber);
        if (moviePtr == nullptr || screenPtr == nullptr) {
            cout << "  Could not create show: movie or screen not found.\n";
            return;
        }
        shows.push_back(Show(nextShowId++, moviePtr, screenPtr, startTime));
    }

    const vector<Movie>& getMovies() const { return movies; }
    vector<Show>& getShows() { return shows; }

    const Movie* findMovieByTitle(const string& title) const {
        for (const Movie& m : movies) {
            if (m.getTitle() == title) return &m;
        }
        return nullptr;
    }

    const Screen* findScreenByNumber(int number) const {
        for (const Screen& s : screens) {
            if (s.getScreenNumber() == number) return &s;
        }
        return nullptr;
    }

    Show* findShowById(int showId) {
        for (Show& s : shows) {
            if (s.getShowId() == showId) return &s;
        }
        return nullptr;
    }

    vector<Show*> getShowsForMovie(const string& title) {
        vector<Show*> result;
        for (Show& s : shows) {
            if (s.getMovie()->getTitle() == title) result.push_back(&s);
        }
        return result;
    }

    // F1: list all movies currently playing
    void listMovies() const {
        cout << "\n";
        for (size_t i = 0; i < movies.size(); i++) {
            cout << "  [" << (i + 1) << "] " << left << setw(14) << movies[i].getTitle()
                 << movies[i].getLanguage() << "  " << movies[i].getDuration() << " min\n";
        }
    }
};

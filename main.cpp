#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include<random>
#include <vector>
using namespace::std;

struct Movie {
    bool inserted = false;
    string name;
    //The Big 3, Copy, Destructor, constructor-S need to be made!
    string genre;
    string director;
    int year;
    double rating;

    Movie(string genre, string director, int year, double rating, string name) {
        this->genre = genre;
        this->director = director;
        this->year = year;
        this->rating = rating;
        this->name = name;
    }
    Movie() {
        this->genre = "";
        this->director = "";
        this->year = 0;
        this->rating = 0.0;
        this->name = "";
    }

    void updateName() {
        this->name = this->name + " (" + to_string(this->year) + ")";
    }

    bool operator<(const Movie& right) const {
        if (this->name.compare(right.name) < 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator>(const Movie& right) const {
        if (this->name.compare(right.name) > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator==(const Movie& right) const {
        if (this->name.compare(right.name) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
};

class unordered_map {
private:
    double lf;
    int hash(int val) {
        return val % cap;
    }

    int linearProbe(int start, Movie& m) {
        if (start >= cap)
            return linearProbe(0, m);
        else if (movies[start].inserted) {
            return linearProbe(start + 1, m);
        }
        return start;
    }
public:
    long long cap;
    Movie* movies;
    int size = 0;
    unordered_map() {
        cap = 10;
        movies = new Movie[cap];
        lf = 0.75;
    }

    void insert(Movie& m) {
        int hashVal = hash(m.name.size());
        if ((*this)[m.name] != nullptr) {
            m.updateName();
        }


        if (!movies[hashVal].inserted) {
            movies[hashVal] = m;
            movies[hashVal].inserted = true;
            size++;
        }
        else {
            int index = linearProbe(hashVal + 1, m);
            movies[index] = m;
            movies[index].inserted = true;
            size++;
        }
        //Load Factor Check
        if (lf < (double)size / cap) {
            Movie* temp = new Movie[cap * 2];
            for (int i = 0; i < cap; i++) {
                temp[i] = movies[i];
            }
            delete[] movies;
            movies = temp;
            cap *= 2;
        }
    }

    //Movie Copy constructor needed
    Movie* operator[](string key) {
        Movie* val = nullptr;
        //search
        int hashVal = hash(key.size());
        if (movies[hashVal].inserted) {
            for (int i = 0; i < cap; i++) {
                if (hashVal >= cap)
                    hashVal = 0;
                if (movies[hashVal].name == key)
                    val = &movies[hashVal];
                hashVal++;
            }
        }
        return val;

    }

    void findSimilarDirectors(string inDirector, string inName) {
        vector<string> simMovies;
        for (int i = 0; i < cap; i++) {
            if (movies[i].director == inDirector && movies[i].name != inName) {
                simMovies.push_back(movies[i].name);
            }
        }
        outputMovieSuggestions(simMovies);
    }

    void findSimilarGenre(string inGenre, string inName) {
        vector<string> simMovies;
        for (int i = 0; i < cap; i++) {
            if (movies[i].genre == inGenre && movies[i].name != inName) {
                simMovies.push_back(movies[i].name);
            }
        }
        outputMovieSuggestions(simMovies);
    }

    void findSimilarYear(double inYear, string inName) {
        vector<string> simMovies;
        for (int i = 0; i < cap; i++) {
            if (movies[i].year == inYear && movies[i].name != inName) {
                simMovies.push_back(movies[i].name);
            }
        }
        outputMovieSuggestions(simMovies);
    }

    void outputMovieSuggestions(vector<string> simMovies) {
        cout << "Here's what you should watch... " << endl << endl;
        if (simMovies.size() < 10) {
            for (int i = 0; i < simMovies.size(); i++) {
                cout << (*this)[simMovies.at(i)]->name << ":" << endl;
                cout << "Release Year: " << (*this)[simMovies.at(i)]->year << endl;
                cout << "Director: " << (*this)[simMovies.at(i)]->director << endl;
                cout << "Genre: " << (*this)[simMovies.at(i)]->genre << endl;
                cout << "Rating: " << (*this)[simMovies.at(i)]->rating << "/10" << endl;
                cout << endl;
            }
        }
        else {
            random_shuffle(simMovies.begin(), simMovies.end());
            for (int i = 0; i < 10; i++) {
                cout << (*this)[simMovies.at(i)]->name << ":" << endl;
                cout << "Release Year: " << (*this)[simMovies.at(i)]->year << endl;
                cout << "Director: " << (*this)[simMovies.at(i)]->director << endl;
                cout << "Genre: " << (*this)[simMovies.at(i)]->genre << endl;
                cout << "Rating: " << (*this)[simMovies.at(i)]->rating << "/10" << endl;
                cout << endl;
            }
        }
    }

    void importData(unordered_map& Movie_Map) {
        ifstream inFS;
        //Title,IMDb Rating,Year,Genres,Directors
        string inTitle;
        string inRating;
        string inYear;
        string inGenre;
        string inDirector;
        int inRating_i;
        double inRating_d;

        inFS.open("/Users/jonathanasbury/CLionProjects/Project 3/MovieSuggester/data/movie_data.csv");
        if (!inFS.is_open()) {
            cout << "Could not open movie data" << endl;
            return;
        }

        getline(inFS, inTitle, ',');
        getline(inFS, inRating, ',');
        getline(inFS, inYear, ',');
        getline(inFS, inGenre, ',');
        getline(inFS, inDirector);

        while (!inFS.eof()) {
            getline(inFS, inTitle, ',');
            getline(inFS, inRating, ',');
            getline(inFS, inYear, ',');
            getline(inFS, inGenre, ',');
            getline(inFS, inDirector);

            if (!inFS.fail()) {
                try {
                    inRating_d = stod(inRating);
                }
                catch (std::invalid_argument) {
                    cout << "Caught Exception: " << inRating << endl;
                    inRating_i = stoi(inRating);
                    inRating_d = (double)inRating_i * 1.0;
                }
                Movie NewMovie = Movie(
                        inGenre,
                        inDirector,
                        stoi(inYear),
                        inRating_d,
                        inTitle
                        );
                Movie_Map.insert(NewMovie);
            }
            else {
                cout << "Failed while importing data" << endl;
                return;
            }
        }
    }
};

struct find_by_name {
    string name;
    find_by_name(const string& inName) {
        name = inName;
    }
    bool operator()(const Movie& movie) {
        return movie.name == name;
    }
};

class ordered_map {
private:
    set<Movie> movie_set;

public:
    ordered_map() {
    }

    void insert(Movie inMovie) {
        if ((*this)[inMovie.name] != nullptr) {
            inMovie.name += " (" + to_string(inMovie.year) + ")";
        }
        movie_set.insert(inMovie);
    }

    const Movie* operator[](string title) {
        set<Movie>::iterator result = find_if(movie_set.begin(), movie_set.end(), find_by_name(title));
        if (result != movie_set.end()) {
            return &(*result);
        }
        else {
            return nullptr;
        }
    }

    void findSimilarDirectors(string inDirector, string inName) {
        set<Movie>::iterator itr;
        vector<string> simMovies;
        for (itr = movie_set.begin(); itr != movie_set.end(); itr++) {
            if (itr->director == inDirector && itr->name != inName) {
                simMovies.push_back(itr->name);
            }
        }
        outputMovieSuggestions(simMovies);
    }

    void findSimilarGenre(string inGenre, string inName) {
        set<Movie>::iterator itr;
        vector<string> simMovies;
        for (itr = movie_set.begin(); itr != movie_set.end(); itr++) {
            if (itr->genre == inGenre && itr->name != inName) {
                simMovies.push_back(itr->name);
            }
        }
        outputMovieSuggestions(simMovies);
    }

    void findSimilarYear(double inYear, string inName) {
        set<Movie>::iterator itr;
        vector<string> simMovies;
        for (itr = movie_set.begin(); itr != movie_set.end(); itr++) {
            if (itr->year == inYear && itr->name != inName) {
                simMovies.push_back(itr->name);
            }
        }
        outputMovieSuggestions(simMovies);
    }

    void outputMovieSuggestions(vector<string> simMovies) {
        cout << "Here's what you should watch... " << endl << endl;
        if (simMovies.size() < 10) {
            for (int i = 0; i < simMovies.size(); i++) {
                cout << (*this)[simMovies.at(i)]->name << ":" << endl;
                cout << "Release Year: " << (*this)[simMovies.at(i)]->year << endl;
                cout << "Director: " << (*this)[simMovies.at(i)]->director << endl;
                cout << "Genre: " << (*this)[simMovies.at(i)]->genre << endl;
                cout << "Rating: " << (*this)[simMovies.at(i)]->rating << "/10" << endl;
                cout << endl;
            }
        }
        else {
            random_shuffle(simMovies.begin(), simMovies.end());
            for (int i = 0; i < 10; i++) {
                cout << (*this)[simMovies.at(i)]->name << ":" << endl;
                cout << "Release Year: " << (*this)[simMovies.at(i)]->year << endl;
                cout << "Director: " << (*this)[simMovies.at(i)]->director << endl;
                cout << "Genre: " << (*this)[simMovies.at(i)]->genre << endl;
                cout << "Rating: " << (*this)[simMovies.at(i)]->rating << "/10" << endl;
                cout << endl;
            }
        }
    }

    void importData(ordered_map& Movie_Map) {
        ifstream inFS;
        //Title,IMDb Rating,Year,Genres,Directors
        string inTitle;
        string inRating;
        string inYear;
        string inGenre;
        string inDirector;
        int inRating_i;
        double inRating_d;

        inFS.open("/Users/jonathanasbury/CLionProjects/Project 3/MovieSuggester/data/fake_data.csv");
        if (!inFS.is_open()) {
            cout << "Could not open movie data" << endl;
            return;
        }

        getline(inFS, inTitle, ',');
        getline(inFS, inRating, ',');
        getline(inFS, inYear, ',');
        getline(inFS, inGenre, ',');
        getline(inFS, inDirector);

        while (!inFS.eof()) {
            getline(inFS, inTitle, ',');
            getline(inFS, inRating, ',');
            getline(inFS, inYear, ',');
            getline(inFS, inGenre, ',');
            getline(inFS, inDirector);

            if (!inFS.fail()) {
                try {
                    inRating_d = stod(inRating);
                }
                catch (std::invalid_argument) {
                    cout << "Caught Exception: " << inRating << endl;
                    inRating_i = stoi(inRating);
                    inRating_d = (double)inRating_i * 1.0;
                }
                Movie NewMovie = Movie(
                        inGenre,
                        inDirector,
                        stoi(inYear),
                        inRating_d,
                        inTitle
                        );
                Movie_Map.insert(NewMovie);
            }
            else {
                cout << "Failed while importing data" << endl;
                return;
            }
        }
    }
};

int main() {
    string alg_input;
    int AlgChoice;
    int searchChoice;
    string inMovie;

    cout << "Welcome to MovieSuggester3001!" << endl;
    cout << "This tool will let you input a movie title and searches for similar movies based on director, genre, or release year" << endl << endl;
    cout << endl;
    cout << "Settings: choose your algorithm" << endl;
    cout << "1) Unordered Map" << endl;
    cout << "2) Ordered Map" << endl;
    cout << "Setting Choice: " << endl;
    getline(cin, alg_input);
    AlgChoice = stoi(alg_input);


    if (AlgChoice == 1) {
        unordered_map Movie_Map = unordered_map();
        Movie_Map.importData(Movie_Map);

        cout << "Input Move Title:" << endl;
        getline(cin, inMovie);

        cout << "Get movie recommendations based on:" << endl;
        cout << "1) Director" << endl;
        cout << "2) Genre" << endl;
        cout << "3) Release Year" << endl;
        cout << "Search Choice:" << endl;
        cin >> searchChoice;

        if (searchChoice == 1) {
            Movie_Map.findSimilarDirectors(Movie_Map[inMovie]->director, Movie_Map[inMovie]->name);
        }
        else if (searchChoice == 2) {
            Movie_Map.findSimilarGenre(Movie_Map[inMovie]->genre, Movie_Map[inMovie]->name);
        }
        else if (searchChoice == 3) {
            Movie_Map.findSimilarYear(Movie_Map[inMovie]->year, Movie_Map[inMovie]->name);
        }

    }
    else if (AlgChoice == 2) {
        ordered_map Movie_Map = ordered_map();
        Movie_Map.importData(Movie_Map);

        cout << "Input Move Title:" << endl;
        getline(cin, inMovie);


        const Movie* TargetMovie = Movie_Map[inMovie];

        cout << "Get movie recommendations based on:" << endl;
        cout << "1) Director" << endl;
        cout << "2) Genre" << endl;
        cout << "3) Release Year" << endl;
        cout << "Search Choice:" << endl;
        cin >> searchChoice;


        if (searchChoice == 1) {
            Movie_Map.findSimilarDirectors(Movie_Map[inMovie]->director, Movie_Map[inMovie]->name);
        }
        else if (searchChoice == 2) {
            Movie_Map.findSimilarGenre(Movie_Map[inMovie]->genre, Movie_Map[inMovie]->name);
        }
        else if (searchChoice == 3) {
            Movie_Map.findSimilarYear(Movie_Map[inMovie]->year, Movie_Map[inMovie]->name);
        }
    }


    cout << "Enjoy your movie!" << endl;


    return 0;
}
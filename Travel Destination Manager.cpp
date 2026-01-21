#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

string toLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

string getCategoryName(int cat) {
    if (cat == 1) return "Attractions";
    else if (cat == 2) return "Activities / Experiences";
    else if (cat == 3) return "Food & Dining";
    else if (cat == 4) return "Accommodation";
    else if (cat == 5) return "Shopping";
    else if (cat == 6) return "Services";
    else if (cat == 7) return "Other";
    else return "Unknown";
}

class Destination {
public:
    string country;
    string city;
    int category;
    string note;
    float rating;
    float cost;

    Destination() {
        country = "";
        city = "";
        category = 0;
        note = "";
        rating = 0;
        cost = 0;
    }

    void display() {
        cout << "Country: " << country
             << " | City: " << city
             << " | Category: " << getCategoryName(category);
        if (note != "-") {
            cout << " | Note: " << note;
        }
        cout << " | Rating: " << rating
             << " | Cost: $" << cost << endl;
    }
};

const int MAX_DEST = 150;

void swapDest(Destination &a, Destination &b) {
    Destination temp = a;
    a = b;
    b = temp;
}

void sortByRatingDesc(Destination dest[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (dest[j].rating < dest[j + 1].rating) {
                swapDest(dest[j], dest[j + 1]);
            }
        }
    }
}

void saveToFile(Destination dest[], int count) {
    ofstream file("destinations.txt");
    if (!file) {
        cout << "Error opening file for saving!\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        file << dest[i].country << "|"
             << dest[i].city << "|"
             << dest[i].category << "|"
             << dest[i].note << "|"
             << dest[i].rating << "|"
             << dest[i].cost << "\n";
    }
    file.close();
    cout << "\nSaved " << count << " destinations successfully.\n";
}

int loadFromFile(Destination dest[]) {
    ifstream file("destinations.txt");
    if (!file) return 0;

    int count = 0;
    string line;

    while (getline(file, line) && count < MAX_DEST) {
        if (line.length() == 0) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        size_t p5 = line.find('|', p4 + 1);

        if (p1 == string::npos || p5 == string::npos) continue;

        dest[count].country  = line.substr(0, p1);
        dest[count].city     = line.substr(p1 + 1, p2 - p1 - 1);
        dest[count].note     = line.substr(p3 + 1, p4 - p3 - 1);

        string catStr   = line.substr(p2 + 1, p3 - p2 - 1);
        string ratStr   = line.substr(p4 + 1, p5 - p4 - 1);
        string costStr  = line.substr(p5 + 1);

        int tempCat = 0;
        float tempRat = 0, tempCost = 0;

        if (sscanf(catStr.c_str(),   "%d",   &tempCat)   == 1)   dest[count].category = tempCat;
        if (sscanf(ratStr.c_str(),   "%f",   &tempRat)   == 1)   dest[count].rating   = tempRat;
        if (sscanf(costStr.c_str(),  "%f",   &tempCost)  == 1)   dest[count].cost     = tempCost;

        if (dest[count].category >= 1 && dest[count].category <= 7) {
            count++;
        }
    }
    file.close();
    return count;
}

void addDestination(Destination dest[], int &count) {
    if (count >= MAX_DEST) {
        cout << "List is full! Maximum " << MAX_DEST << " destinations.\n";
        return;
    }

    Destination &d = dest[count];

    cout << "\nEnter country: ";
    getline(cin, d.country);

    cout << "Enter city: ";
    getline(cin, d.city);

    cout << "\nChoose category:\n";
    cout << "1. Attractions\n";
    cout << "2. Activities / Experiences\n";
    cout << "3. Food & Dining\n";
    cout << "4. Accommodation\n";
    cout << "5. Shopping\n";
    cout << "6. Services\n";
    cout << "7. Other\n";
    cout << "Your choice (1-7): ";

    cin >> d.category;
    cin.ignore(1000, '\n');

    while (d.category < 1 || d.category > 7) {
        cout << "\nInvalid! Please select 1-7:\n";
        cout << "1. Attractions  2. Activities / Experiences\n";
        cout << "3. Food & Dining  4. Accommodation\n";
        cout << "5. Shopping  6. Services  7. Other\n";
        cout << "Your choice: ";
        cin >> d.category;
        cin.ignore(1000, '\n');
    }

    cout << "Selected: " << getCategoryName(d.category) << endl;

    cout << "Note (press enter for none): ";
    getline(cin, d.note);
    if (d.note.length() == 0) d.note = "-";

    cout << "Rating (0-5): ";
    cin >> d.rating;
    cin.ignore(1000, '\n');

    while (d.rating < 0 || d.rating > 5) {
        cout << "Invalid! Please enter 0-5: ";
        cin >> d.rating;
        cin.ignore(1000, '\n');
    }

    cout << "Cost (in $): ";
    cin >> d.cost;
    cin.ignore(1000, '\n');

    while (d.cost < 0) {
        cout << "Invalid! Cost must be >= 0: ";
        cin >> d.cost;
        cin.ignore(1000, '\n');
    }

    count++;
    sortByRatingDesc(dest, count);
    cout << "Destination added and list sorted by rating (highest first).\n";
}

void viewAll(Destination dest[], int count) {
    if (count == 0) {
        cout << "No destinations yet.\n";
        return;
    }
    cout << "\n--- All Destinations (sorted by rating descending) ---\n";
    for (int i = 0; i < count; i++) {
        dest[i].display();
    }
    cout << "------------------------\n";
}

void searchByCity(Destination dest[], int count) {
    string city;
    cout << "\nEnter city name: ";
    getline(cin, city);

    string lowCity = toLower(city);

    char filterCat;
    cout << "Do you want to filter by category too? (y/n): ";
    cin >> filterCat;
    cin.ignore(1000, '\n');

    int selectedCat = 0;
    if (tolower(filterCat) == 'y') {
        cout << "\nChoose category:\n";
        cout << "1. Attractions\n2. Activities / Experiences\n3. Food & Dining\n";
        cout << "4. Accommodation\n5. Shopping\n6. Services\n7. Other\n";
        cout << "Your choice (1-7): ";
        cin >> selectedCat;
        cin.ignore(1000, '\n');

        while (selectedCat < 1 || selectedCat > 7) {
            cout << "Invalid! Enter 1-7: ";
            cin >> selectedCat;
            cin.ignore(1000, '\n');
        }
    }

    cout << "\nResults for city: " << city;
    if (selectedCat > 0) {
        cout << " (only " << getCategoryName(selectedCat) << ")";
    }
    cout << ":\n";

    bool found = false;
    for (int i = 0; i < count; i++) {
        bool cityMatch = (toLower(dest[i].city) == lowCity);
        bool catMatch = (selectedCat == 0 || dest[i].category == selectedCat);

        if (cityMatch && catMatch) {
            dest[i].display();
            found = true;
        }
    }

    if (!found) {
        cout << "No matching destinations found.\n";
    }
}

void searchByCategory(Destination dest[], int count) {
    int catChoice;
    cout << "\nChoose category to search:\n";
    cout << "1. Attractions\n";
    cout << "2. Activities / Experiences\n";
    cout << "3. Food & Dining\n";
    cout << "4. Accommodation\n";
    cout << "5. Shopping\n";
    cout << "6. Services\n";
    cout << "7. Other\n";
    cout << "Your choice (1-7): ";
    cin >> catChoice;
    cin.ignore(1000, '\n');

    while (catChoice < 1 || catChoice > 7) {
        cout << "Invalid! Please select 1-7: ";
        cin >> catChoice;
        cin.ignore(1000, '\n');
    }

    char filterCity;
    cout << "Do you want to filter by a specific city? (y/n): ";
    cin >> filterCity;
    cin.ignore(1000, '\n');

    string selectedCity = "";
    if (tolower(filterCity) == 'y') {
        cout << "Enter city name: ";
        getline(cin, selectedCity);
    }

    string lowCity = toLower(selectedCity);

    cout << "\nResults for category: " << getCategoryName(catChoice);
    if (selectedCity.length() > 0) {
        cout << " (only in " << selectedCity << ")";
    }
    cout << ":\n";

    bool found = false;
    for (int i = 0; i < count; i++) {
        bool catMatch = (dest[i].category == catChoice);
        bool cityMatch = (selectedCity.length() == 0 || toLower(dest[i].city) == lowCity);

        if (catMatch && cityMatch) {
            dest[i].display();
            found = true;
        }
    }

    if (!found) {
        cout << "No matching destinations found.\n";
    }
}

int main() {
    Destination destinations[MAX_DEST];
    int count = loadFromFile(destinations);
    sortByRatingDesc(destinations, count);

    cout << "=====================================\n";
    cout << "   Welcome to Travel Destination Manager\n";
    cout << "=====================================\n";
    if (count == 0) {
        cout << "No destinations loaded (file empty or not found).\n";
    } else if (count == 1) {
        cout << "Loaded and sorted 1 destination from file.\n";
    } else {
        cout << "Loaded and sorted " << count << " destinations from file.\n";
    }

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Add new destination\n";
        cout << "2. View all destinations (sorted by rating)\n";
        cout << "3. Search by city (with optional category filter)\n";
        cout << "4. Search by category (with optional city filter)\n";
        cout << "5. Save and exit\n";
        cout << "Your choice (1-5): ";

        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) {
            addDestination(destinations, count);
        }
        else if (choice == 2) {
            viewAll(destinations, count);
        }
        else if (choice == 3) {
            searchByCity(destinations, count);
        }
        else if (choice == 4) {
            searchByCategory(destinations, count);
        }
        else if (choice == 5) {
            saveToFile(destinations, count);
            cout << "\nThank you! Goodbye!\n";
        }
        else {
            cout << "Invalid choice! Please enter 1-5.\n";
        }

    } while (choice != 5);

    return 0;
}

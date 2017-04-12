#include <iostream>
#include <string>
#include <fstream>
#include <list>

using namespace std;

const int READ_ERROR = 100;
const int WRITE_ERROR = 200;
const int ARRAY_SIZE = 100;

class Restaurant
{
    string restaurantName;
    string typeOfFood;
    string location;
    string rating;


public:
    Restaurant();
    Restaurant(string name, string type, string location, string rate);
    void captureNewRestaurant();
    void showRestaurant();
    int saveRestaurant(ofstream &outfile);
    bool isRestaurantEqual(Restaurant *name);
    friend class RestaurantList;
};


class RestaurantList {
protected:
    Restaurant** list;
    int numberOfRestaurants;
    int sizeOfList;
    int reallocateArray();

public:
    RestaurantList();
    ~RestaurantList();
    void getUserInput();
    void showRestaurantList();
    int saveRestaurantList(string filename);
    int readRestaurantList(string filename);
    void addNewRestaurant();
};

Restaurant::Restaurant() {
    restaurantName = "";
    typeOfFood = "";
    location = "";
    rating = "";
}

Restaurant::Restaurant(string name, string type, string location, string rate) {
    restaurantName = name;
    typeOfFood = type;
    location = location;
    rating = rate;
}

bool Restaurant::isRestaurantEqual(Restaurant *name) {
    if (restaurantName == name->restaurantName) {
        return 1;
    } else {
        return 0;
    }
}

void Restaurant::captureNewRestaurant() {
    cout << "Name of restaurant is: ";
    getline(cin, restaurantName);
    cout << "Restaurant food type is: ";
    getline(cin, typeOfFood);
    cout << "Location of restaurant: ";
    getline(cin, location);
    cout << "Rating of restaurant (1-5): ";
    getline(cin, rating);
}

void Restaurant::showRestaurant() {
    cout << "Name: " << restaurantName << endl;
    cout << "\nFood: " << typeOfFood;
    cout << "\nLocation: " << location;
    cout << "\nRating: " << rating;
}

int Restaurant::saveRestaurant(ofstream &outfile) {
    if (outfile.is_open()) {
        outfile << restaurantName << "|" << typeOfFood << "|" << location << "|"
                << rating << endl;
        return 0;
    }
    else
        return WRITE_ERROR;
}

RestaurantList::RestaurantList() {
    list = new Restaurant*[ARRAY_SIZE];
    numberOfRestaurants = 0;
    sizeOfList = ARRAY_SIZE;
}

RestaurantList::~RestaurantList() {
    for (int i = 0; i < numberOfRestaurants; i++) {
        delete list[i];
    }
    delete[]list;
}

int RestaurantList::reallocateArray() {
    Restaurant** temp;
    temp = new Restaurant*[sizeOfList + ARRAY_SIZE];
    sizeOfList = sizeOfList + ARRAY_SIZE;
    for (int i = 0; i < numberOfRestaurants; i++) {
        temp[i] = list[i];
    }
    delete[]list;
    list = temp;
    return 0;
}

void RestaurantList::addNewRestaurant() {
    Restaurant* myRestaurant;
    myRestaurant = new Restaurant();
    bool found = false;

    myRestaurant->captureNewRestaurant();

    for (int i = 0; i < numberOfRestaurants; i++) {
        if (list[i]->isRestaurantEqual(myRestaurant)) {
            cout << "\nDuplicate Restaurant Found! Please Enter Another Restaurant" << endl;
            found = true;
        }
    }
    if (found == false) {
        list[numberOfRestaurants] = myRestaurant;
        numberOfRestaurants++;
    }
}

void RestaurantList::getUserInput() {
    string answer = "Y";
    cout << "\nEnter Restaurant Y/N?" << endl;
    getline(cin, answer);
    while (answer == "Y" || answer == "y") {
        Restaurant* myRestaurant = new Restaurant();
        addNewRestaurant();
        cout << "\nEnter another Restaurant Y/N?" << endl;
        getline(cin, answer);
    }
}

void RestaurantList::showRestaurantList() {
    for (int i = 0; i < numberOfRestaurants; i++) {
        list[i]->showRestaurant();
    }
}


int RestaurantList::saveRestaurantList(string filename) {
    ofstream output(filename, ios::trunc);
    if (output) {
        for (int i = 0; i < numberOfRestaurants; i++)
        {
            list[i]->saveRestaurant(output);
        }
        output.close();
    } else {
        return WRITE_ERROR;
    }

    return 0;
}

int RestaurantList::readRestaurantList(string filename) {
    string iname, name, type, location, rating, irate;
    int count = 0;
    ifstream infile(filename, ios::in);
    if (!infile) {
        cout << "File could not be opened" << endl;
        return READ_ERROR;
    }
    while (!infile.eof()) {
        if (numberOfRestaurants == sizeOfList) {
            reallocateArray();
        }
        getline(infile, iname, '|');
        if (!iname.empty()) {
            getline(infile, name, '|');
            getline(infile, type, '|');
            getline(infile, location, '|');
            getline(infile, rating);
            list[numberOfRestaurants] = new Restaurant(iname, type, location, rating);
            numberOfRestaurants++;
        }
    }
    infile.close();
    return 0;
}

int main() {

    string answer,filePath;
    int error;

    RestaurantList restaurantList;

    cout << "Enter the full path to file" << endl;

    getline(cin, filePath);
    ifstream file(filePath);
    error = restaurantList.readRestaurantList(filePath);
    if (error) {
        cout << "Error reading file. It may not be created yet. Would you like to still continue? (Y/N) : ";
        getline(cin, answer);
        if (answer != "y" && answer != "Y")
        { return 1; }
    }
    restaurantList.getUserInput();
    restaurantList.saveRestaurantList(filePath);
    restaurantList.showRestaurantList();
    return 0;
}
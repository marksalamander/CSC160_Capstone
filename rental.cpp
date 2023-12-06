#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include "rental.h"

static std::string divider = "---------------------------------------\n";

// Deletes any empty lines in the text file and shifts lines up.
void rental::blank_line(const char* file_name)
{
    std::ifstream inf(file_name);

    std::ofstream of;
    // creates a temporary file
    of.open("temp.txt", std::ios::out);

    std::string line;
    // sifts through occuppied lines within the text file
    while (getline(inf, line))
    {
        if (line.length() == 0){
            // while a line is blank, the next line is fetched.
            getline(inf, line);
        }
        // Lines that are occupied (not empty) are written to the temp file
        of << line << std::endl;
    }
    of.close();
    inf.close();
    // original file is deleted
    remove(file_name);
    // temp file is renamed to match the original file's name
    rename("temp.txt", file_name);
}

// The line number indicated by 'n' is deleted from the file
void rental::delete_line(const char* file_name, int n) {
    std::ifstream inf(file_name);

    std::ofstream of;
    // creates a temporary file
    of.open("temp.txt", std::ios::out);

    char c;
    int lineNum = 1;
    // sifts through each line within the text file
    while (inf.get(c))
    {
        // if end of line is reached, lineNum is incremented
        if (c == '\n') {
            lineNum++;
        }

        // All lines that are not the line at line number 'n' is written
        // to the temp file.
        if (lineNum != n) {
            of << c;
        }
    }

    of.close();
    inf.close();
    // original file is deleted
    remove(file_name);
    // temp file is renamed to match the original file's name
    rename("temp.txt", file_name);
}

// The database text file is displayed on screen through this function.
// (function is only accessible to the admin after login)
void rental::displayInfo() {
    std::string line;
    int lineNum = 1;
    std::string cars[8];

    // all blank lines are deleted from the text file.
    blank_line("database.txt");

    std::ifstream inf;
    inf.open("database.txt");

    std::cout << divider;
    // If the first line is empty (since all blank lines are deleted before this code),
    // the rest of the file should be empty as well.
    getline(inf,line);
    if(line.length() == 0){
        inf.close();
        std::cout << "No users currently renting" << std::endl << divider;
        system("pause");
        system("cls");
        menu();
    }
    // Returns to the top of the text file for selecting a car later on.
    inf.clear();
    inf.seekg(0);
    
    // All lines within the text file are outputted to the screen.
    while (getline(inf, line)) {
        std::cout << lineNum << " - " << line << std::endl;
        lineNum++;
    }
    inf.close();
    std::cout << divider;

    system("pause");
    system("cls");

    menu();
    
}

void rental::menu() {
    /* 
    Depending on who is using the application, the user selects whether they
    are the user or the admin. If they are a user, the program calls the function
    user(), and if they are an admin, the program redirects to the function admin().
    */

    int userInput;
    
    std::cout << "======================MENU======================\n";
    std::cout << "1. User" << std::endl;
    std::cout << "2. Admin" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Input: ";
    std::cin.clear();
    std::cin.sync();
    std::cin >> userInput;

    while((userInput > 3) || (userInput < 1)){
        std::cout << "Invalid input, try again: ";
        std::cin.clear();
        std::cin.sync();
        std::cin >> userInput;
    }

    if (userInput == 1) {
        system("cls");
        user();
    }

    else if (userInput == 2) {
        system("cls");
        admin();
    }

    else if (userInput == 3) {
        exit(EXIT_SUCCESS);
    }
}

void rental::admin() {
    /*
    To access the admin controls, a password will be requested. Upon entering the correct
    password, the admin will be able to see two options. The first option will display the
    basic user information of those renting a car. The second option will simply be an
    option to go back to the menu.
    */

    int userInput;
    std::string password = "password1234";
    std::string inputtedPassword = "";
    std::cout << "======================ADMIN======================\n";

    // While loop continues until the correct password is entered or user
    // returns to the menu.
    while (inputtedPassword != password) {
        std::cout << "Enter Password (enter '!b' to go back to menu): ";
        std::cin >> inputtedPassword;

        // Allows the user to return to the menu
        if (inputtedPassword == "!b") {
            system("cls");
            menu();
        }

        else if(inputtedPassword != password){
            std::cout << "Password is incorrect, try again." << std::endl;
        }

        // Function continues if password is correct.
        else if (inputtedPassword == password) {
            continue;
        }
    }

    if (inputtedPassword == password) {
        system("cls");
        std::cout << "======================ADMIN======================\n";
        std::cout << "1. Display Information" << std::endl;
        std::cout << "2. Menu" << std::endl;
        std::cout << "Input: ";
        std::cin >> userInput;

        if (userInput == 1) {
            displayInfo();
        }

        else if (userInput == 2) {
            system("cls");
            menu();
        }
    }
}

void rental::user() {
    /*
    The user will choose between the option of renting a car or returning a rental car.
    The new rental option will have the program call the function newCar(), and the return
    rental option will have the program call the function returnCar(). There will also be
    an option to return to the main menu.
    */

    int userInput;

    std::cout << "======================USER======================\n";
    std::cout << "1. New Rental" << std::endl;
    std::cout << "2. Return Rental" << std::endl;
    std::cout << "3. Menu" << std::endl;
    std::cout << "Input: ";
    std::cin >> userInput;

    while((userInput > 3) || (userInput < 1)){
        std::cout << "Invalid input, try again: ";
        std::cin.clear();
        std::cin.sync();
        std::cin >> userInput;
    }

    if (userInput == 1) {
        system("cls");
        newCar();
    }

    else if (userInput == 2) {
        system("cls");
        returnCar();
    }

    else if (userInput == 3) {
        system("cls");
        menu();
    }
}

// Function that removes car from list of available rentals after being rented.
void rental::removeCar(const char *file_name){
    std::string line;
    std::ifstream inf(file_name);
    std::ofstream ofRented;
  
    std::ofstream ofCars;
    // Creates a temporary text file
    ofCars.open("temp.txt", std::ios::out);
  
    // Ouputs all lines within the text file
    while(getline(inf,line))
    { 
        if(line == getCar()){
            // if the text on a line matches the selected car,
            // 'rentedCars.txt' is opened and the selected car 
            // is written to the text file
            ofRented.open("rentedCars.txt", std::ios::app);
            ofRented << getCar() << std::endl;
            ofRented.close();
            // Moves to the next line in the text file
            getline(inf, line);
        }   

        // All lines that don't match the selected car are
        // written to the temporary text file.
        ofCars << line << std::endl;
    }
    ofCars.close();  
    inf.close();  
    // original file is deleted
    remove(file_name);
    // temp file is renamed to match the original file's name
    rename("temp.txt", file_name);
}

// Function that adds car back into list of available cars after return.
void rental::addCar(){
    std::string line;
    int lineNum = getLineNum();
    int tempNum = 1;

    std::ifstream infRented;

    std::ifstream infCars;
    std::ofstream ofCars;

    infRented.open("rentedCars.txt");

    // Sifts through all lines within 'rentedCars.txt'
    while (getline(infRented, line)) {
        // Checks if line number matches the line number given through the newCar()
        // function after the user selects car.
        if (tempNum == lineNum) {
            // Car being returned is written back to the list of available cars.
            setCar(line);
            ofCars.open("cars.txt", std::ios::app);
            ofCars << getCar() << std::endl;
            ofCars.close();
            infCars.close();
            infRented.close();

            // After renting a car, the information written to each text file should
            // be placed on the same line numbers as each other. When returning a car, 
            // the information at this line number is deleted through the delete_line()
            // function.
            delete_line("users.txt", lineNum);
            delete_line("database.txt", lineNum);
            delete_line("rentedCars.txt", lineNum);
        }
        // Increments the line number if it does not match lineNum
        tempNum++;
    }

    std::cout << divider;
    std::cout << "Thank you for renting with us!" << std::endl;
    
    system("pause");
    system("cls");

    menu();
}

std::string rental::newCar() {
    /*
    Program will display a list of cars that are available to rent. The user will select
    an available car, where they will then be taken to the total() function.
    */
    char userInput;
    std::string car;
    std::string line;
    int lineNum = 1;
    int selectedCar;
    // All blank lines are removed from 'cars.txt'
    blank_line("cars.txt");

    std::cout << "======================RENT======================\n";

    std::ifstream inf;
    inf.open("cars.txt");

    // Checks if the file containing list of available cars is empty,
    // if it is, the user is taken back to the menu.
    getline(inf,line);
    if(line.length() == 0){
        inf.close();
        std::cout << "No rental cars currently availale" << std::endl << divider;
        system("pause");
        system("cls");
        menu();
    }
    // Returns to the top of the text file for selecting a car later on.
    inf.clear();
    inf.seekg(0);

    std::cout << "Select car to rent:" << std::endl;
    // All available cars found in 'cars.txt' are displayed on the screen
    while (getline(inf, line)) {
        std::cout << lineNum << " - " << line << std::endl;
        lineNum++;
    }
    // Returns to the top of the text file for selecting a car later on.
    inf.clear();
    inf.seekg(0);

    std::cout << "Input (Enter '0' to go back): ";
    std::cin >> selectedCar;

    // Returns to menu
    if (selectedCar == 0) {
        system("cls");
        menu();
    }

    lineNum = 1;
    // Sifts through each line of 'cars.txt'
    while (getline(inf, line)) {
        if (lineNum == selectedCar) {
            // user confirms the selected car to rent.
            std::cout << "Rent " << line << "? (Y/N): ";
            std::cin >> userInput;
            // input is converted to uppercase
            userInput = toupper(userInput);

            // while loop until a valid input ('Y' or 'N') is given.
            while ((userInput != 'Y') || (userInput != 'N')) {
                if (userInput == 'Y') {
                    // Selected car is rented and removed from list of available cars.
                    car = line;
                    setCar(car);
                    inf.close();

                    std::cout << divider;
                    total();
                }

                // returns user to beginning of function
                else if (userInput == 'N') {
                    inf.close();
                    system("cls");
                    newCar();
                }

                else {
                    std::cout << "Please enter a valid input." << std::endl;
                    std::cout << "Rent " << line << "? (Y/N): ";
                    std::cin >> userInput;
                    userInput = toupper(userInput);
                }
            }
        }
        lineNum++;
    }
    return car;
}

void rental::returnCar() {
    /*
    Program will display cars currently being rented out, user will select car they are 
    returning. The number inputted corresponds to the line number of the car and user 
    in their respective text files. If the name that the user input matches the name 
    at the line number in the text file, the car will be returned.
    */
    std::string userName;
    std::string line;
    int selectedCar;
    int lineNum = 1;

    std::ifstream inf;

    std::cout << "======================RETURN======================\n";
    // blank lines are deleted from the text files.
    blank_line("rentedCars.txt");
    blank_line("users.txt");
    blank_line("database.txt");
    inf.open("rentedCars.txt");

    // If the first line is empty (since all blank lines are deleted before this code),
    // the rest of the file should be empty as well.
    getline(inf,line);
    if(line.length() == 0){
        inf.close();
        std::cout << "No users currently renting" << std::endl << divider;
        system("pause");
        system("cls");
        menu();
    }
    // Returns to the top of the text file for selecting a car later on.
    inf.clear();
    inf.seekg(0);
    
    // Car currently being rented is outputted.
    while (getline(inf, line)) {
        std::cout << lineNum << " - " << line << std::endl;
        lineNum++;
    }

    std::cout << "Select car you are returning (Enter '0' to go back): ";
    std::cin >> selectedCar;

    // Allows the user to return to the menu
    if (selectedCar == 0) {
        system("cls");
        menu();
    }
    while((selectedCar > lineNum) || (selectedCar < 0)){
        std::cout << "Invalid input, try again" << std::endl;
        system("pause");
        system("cls");
        returnCar();
    }

    inf.close();

    std::cin.ignore();
    std::cout << "Please enter your name: ";
    std::cin.clear();
    std::cin.sync();
    getline(std::cin, userName);

    // Checks to make sure only the letters are typed for the name.
    for(int i = 0; i < userName.length(); i++){
        while(isdigit(userName[i])){
            std::cout << "Please only enter letters: ";
            std::cin.clear();
            std::cin.sync();
            getline(std::cin, userName);
        }
    }

    inf.open("users.txt");
    lineNum = 1;
    // Searches text file for line number user inputs according to the car selected to return.
    while (getline(inf, line)) {
        if (lineNum == selectedCar) {
            if(line == userName) {
                // Name matches the name at the given line number
                setLineNum(lineNum);
                inf.close();
                // selected car is returned to the list of available cars
                addCar();

                std::cout << divider;
                std::cout << "Thank you for renting with us!" << std::endl;
                std::cout << divider;
                
                system("pause");
                system("cls");

                menu();
            }
            else{
                // Name does not match the name at the given line number
                inf.close();
                std::cout << "Name entered does not match name in system, try again" << std::endl;
                system("pause");
                system("cls");
                returnCar();
            }
        }
        lineNum++;
    }
}

void rental::setCar(std::string car){
    m_car = car;
}

std::string rental::getCar(){
    return m_car;
}

void rental::setLineNum(int num) {
    m_lineNum = num;
}

int rental::getLineNum() {
    return m_lineNum;
}

void rental::total() {
    /*
    A total is calculated based on the number of days a car is rented,
    as specified by the user. 
    */
    int rentalPeriod;
    int rate;
    char userInput;

    std::cout << "How many days will you be renting? (Enter '0' to return to menu): ";
    // Checks that only an integer is inputted
    while (!(std::cin >> rentalPeriod)) {
        // Clears cin if input is invalid
        std::cin.clear();
        // Ignores all invalid values
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Reprompts the user until valid input is entered
        std::cout << "Please only enter numbers: ";
    }
    if(rentalPeriod == 0){
        system("pause");
        system("cls");
        menu();
    }
    // The rental period inputted must be less than 31 days
    while (rentalPeriod > 31) {
        std::cout << "Rentals may only be rented up to 31 days" << std::endl;
        std::cout << "How many days will you be renting?: ";
        std::cin >> rentalPeriod;
    }
    rate = rentalPeriod * 52;

    std::cout << "Total: $" << rate << std::endl;

    std::cout << "Is this total of $" << rate << " okay? (Y/N): ";
    std::cin.clear();
    std::cin.sync();
    std::cin >> userInput;
    userInput = toupper(userInput);

    while ((userInput != 'Y') || (userInput != 'N')) {

        if (userInput == 'Y') {
            std::cout << divider;
            inputInfo();
        }

        else if (userInput == 'N') {
            std::cout << divider;
            total();
        }

        else {
            std::cout << "Please enter a valid input." << std::endl;
            std::cout << "Is this total of $" << rate << " okay? (Y/N): ";
            std::cin >> userInput;
            userInput = toupper(userInput);
        }
    }
}

void rental::inputInfo() {
    /*
    User is asked to input necessary information, such as their name and phone number.
    If the function is fiven the correct information, it will write to a text file acting
    as the database, and the user is taken back to the menu.
    */
    std::string userName;
    std::string phone;
    bool check = false;

    std::cin.ignore();
    std::cout << "Please enter your name: ";
    std::cin.clear();
    std::cin.sync();
    getline(std::cin, userName);

    // Checks to make sure only the letters are typed for the name.
    for(int i = 0; i < userName.length(); i++){
        while(isdigit(userName[i])){
            std::cout << "Please only enter letters: ";
            std::cin.clear();
            std::cin.sync();
            getline(std::cin, userName);
        }
    }

    // Checks that only digits are inputted for the phone number
    while (!check)
    {
        check = true;

        std::cout << "Please enter a 10-digit phone number: ";
        std::cin.clear();
        std::cin.sync();
        std::cin >> phone;

        // Phone number must be 10 digits long
        while ((phone.length() != 10)){
            std::cout << "Invalid input, please enter a 10-digit phone number: ";
            std::cin.clear();
            std::cin.sync();
            std::cin >> phone;
        }

        // Phone number must contain only integers
        for (int i = 0; i < 10; i++)
        {
            while(!isdigit(phone[i])){
                std::cout << "Invalid input, please enter a 10-digit phone number: ";
                std::cin.clear();
                std::cin.sync();
                std::cin >> phone;
            }
        }
    }

    std::ofstream of;
    // The user's name is written to the "user" text file
    of.open("users.txt", std::ios::app);
    of << userName << std::endl;
    of.close();

    // Removes car from list of available cars
    removeCar("cars.txt");

    // Writes to the "database" text file with all information
    of.open("database.txt", std::ios::app);
    of << userName << std::string(20 - userName.length(), ' ')
        << phone << std::setw(25)
        << getCar() << std::endl;
    of.close();

    // Deletes any empty lines in the text file and shifts lines up.
    blank_line("database.txt");
    blank_line("users.txt");

    std::cout << divider;
    std::cout << "Thank you for renting!" << std::endl;
    std::cout << divider;

    system("pause");
    system("cls");
    menu();
}
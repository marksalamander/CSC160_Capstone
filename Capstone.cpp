// Capstone
// CSC161 C++ Programming
// Mark Alexander
// Dr. Renee Carroll

/*
This program is a car rental system, allowing a user to rent a car
from a given list. The user will input relevant user information to
store in the database. An admin may be able to login and have this user
information displayed within the program. The program also allows the user
to return the car, where it will be made available again to rent for other 
users.
*/

#include <iostream>
#include "rental.h"

int main()
{
    rental rental;

    rental.menu();

    system("pause");
    return 0;
}
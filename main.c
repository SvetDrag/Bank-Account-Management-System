#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max user accounts that can be created 
#define MAX_USERS 100
// Name of the file with data bases
#define FILENAME "accounts.txt"

// -- Project structure --
// Template for every user
typedef struct {
    char firstName[50];         // Name of the user
    char middleName[50];        // Middle name of the user
    char lastName[50];          // Last name of the user
    char accountNumber[20];     // Bank account number 
    double balance;             // Available funds
    int pin;                    // PIN code of the bank account
} User;

// -- Global variables --
// for easy access to all functions
User user[MAX_USERS];       // Array with all of users loaded from .txt file
int userCount = 0;          // Number of loaded users
int currentUserIndex = -1;  // The Index of the currently loaded user
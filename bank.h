#ifndef BANK_H // if bank_h is not defined 
#define BANK_H // define bank_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Max user accounts that can be created
#define MAX_USERS 100
// Name of the file with data bases
#define FILENAME "accounts.txt"

// --> Project structure <--
// Template for every user
typedef struct
{
    char firstName[50];     // Name of the user
    char middleName[50];    // Middle name of the user
    char lastName[50];      // Last name of the user
    char accountNumber[20]; // Bank account number
    double balance;         // Available funds
    char pin[7];            // PIN code of the bank account
} User;
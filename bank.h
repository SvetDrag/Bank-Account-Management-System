#ifndef BANK_H // if bank_h is not defined 
#define BANK_H // define bank_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// Max user accounts that can be created
#define MAX_USERS 100
// Name of the file with data bases
#define FILENAME "data/accounts.txt"
// Caesar key for the cipher 
#define CAESAR_KEY 3

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

// --> Global variables <--
// for easy access to all functions
// 'extern variable' => the variable is somewhere in '.c' file; not located memory here
extern User users[MAX_USERS];   // Array with all of users loaded from .txt file
extern int userCount;           // Number of loaded users
extern int currentUserIndex;    // The Index of the currently loaded user


// --> Functions <--
void loadUsersFromFile();
void saveUsersToFile();
int login();
int verifyPin();
int isValidPin(char *pin);
void registerUser();
void showBalance();
void depositMoney();
void withdrawMoney();
void transferMoney();
void changePin();
void exportStatementHTML();
void logTransaction(int userIndex, const char *type, double amount);
void caesarCipher(char *pin, int mode);
int openFile(char *filename);

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int pin;                // PIN code of the bank account
} User;

// --> Global variables <--
// for easy access to all functions
User users[MAX_USERS];     // Array with all of users loaded from .txt file
int userCount = 0;         // Number of loaded users
int currentUserIndex = -1; // The Index of the currently loaded user

// --> Functions <--
void loadUsersFromFile();
void saveUsersToFile();
int login();
int verifyPin();
void registerUser();
void showBalance();
void depositMoney();
void withdrawMoney();
void transferMoney();
void changePin();

// --> Main function <--
int main()
{
    loadUsersFromFile();
    int startChoice;

    do
    {
        printf("\n === BANK ACCOUNT MANAGEMENT SYSTEM ===\n");
        printf("\n --> MAIN MENU <--\n");
        printf("1. Enter yo your account\n");
        printf("2. Register a new user\n");
        printf("0. Exit\n");
        scanf("%d", &startChoice); // Scan the user input and point it to startChoice variable

        switch (startChoice)
        {
        case 1:
            // LogIn logic
            if (login())
            {
                // --> INSIDE MENU FOR BANK OPERATIONS <--
                int userChoice;

                // Loop while user choice is different from '0' (Exit from bank account system)
                do
                {
                    printf("\n--- USER MENU (%s) ---\n", users[currentUserIndex].accountNumber);
                    printf("1. Withdrawn Money\n");
                    printf("2. Deposit Money\n");
                    printf("3. Transfer to Another User\n");
                    printf("4. Show Balance\n");
                    printf("5. Change PIN\n");
                    printf("0. Exit from the profile (Logout)\n");
                    printf("Your choice: ");
                    scanf("%d", &userChoice);

                    // Invoke function for chosen bank operation and break
                    switch (userChoice)
                    {
                    case 1:
                        withdrawMoney();
                        break;
                    case 2:
                        depositMoney();
                        break;
                    case 3:
                        transferMoney();
                        break;
                    case 4:
                        showBalance();
                        break;
                    case 5:
                        changePin();
                        break;
                    case 0:
                        printf("Exit from the profile...\n");
                        break;
                    default:
                        printf("Invalid choice!\n");
                    }
                } while (userChoice != 0);
            }
            else
            {
                printf("Wrong account number and/or PIN code! Try again or contact our bank's support department.\n");
            }
            break;

        case 2:
            registerUser();
            break;

        case 0:
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid choice!\n");
        }
    } while (startChoice != 0);

    return 0;
}

// --> Implementation of Functions <--

// Function for read the file (data bases with all user information)
void loadUsersFromFile()
{
    FILE *fp = fopen(FILENAME, "r"); // Open file for reading

    if (fp == NULL)
    {
        printf("The data file can't be found...\n");
        return;
    }

    userCount = 0;
    // Read each line from the data file until the end of the file is reached (EOF)
    while (fscanf(fp, "%s %s %s %s %lf %d",
                  users[userCount].firstName,
                  users[userCount].middleName,
                  users[userCount].lastName,
                  users[userCount].accountNumber,
                  &users[userCount].balance,
                  &users[userCount].pin) != EOF)
    {
        userCount++;
    }

    fclose(fp); // CLOSE THE DATA FILE
}

// Function for save the file (rewrite)
void saveUsersToFile()
{
    FILE *fp = fopen(FILENAME, "w"); // Open file for writing

    if (fp == NULL)
    {
        printf("ERROR with saving the data!\n");
        return;
    }

    for (int i = 0; i < userCount; i++)
    {
        fprintf(fp, "%s %s %s %s %.2f %d\n",
                users[i].firstName,
                users[i].middleName,
                users[i].lastName,
                users[i].accountNumber,
                users[i].balance,
                users[i].pin);
    }

    fclose(fp);
}

// Function for logIn to user account
int login(){
    char inputAcc[20];
    int inputPin;

    printf("\n--- LOGIN TO THE SYSTEM ---\n");
    printf("Please enter a bank account number.: ");
    scanf("%s", inputAcc);
    printf("Please enter a PIN code: ");
    scanf("%d", &inputPin);

    // Loop through the user array to find a match
    for (int i = 0; i < userCount; i++) {
        // strcmp compare 2 Strings. If return 0 => they are equal.
        if (strcmp(users[i].accountNumber, inputAcc) == 0 && users[i].pin == inputPin) {
            currentUserIndex = i; // Remember witch user is enter
            printf("You have successfully logged into your account!\n");
            return 1; // Return true
        }
    }

    return 0; // Return false
}

// Function for verifying the PIN code before bank operation
int verifyPin(){
    int pinCheck;
    printf("For confirmation, please enter your PIN code: ");
    scanf("%d", &pinCheck);

    if (pinCheck == users[currentUserIndex].pin) {
        return 1; // PIN code is correct (verified)
    } else {
        printf("Грешен PIN код! Операцията е отказана.\n");
        return 0; // PIN code is wrong
    }
}

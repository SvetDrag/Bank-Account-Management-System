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

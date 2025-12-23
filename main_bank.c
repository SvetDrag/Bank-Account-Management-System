#include "bank.h"

// --> Main function <--
int main()
{
    loadUsersFromFile();
    int startChoice;

    do
    {
        printf("\n === BANK ACCOUNT MANAGEMENT SYSTEM ===\n");
        printf("\n --> MAIN MENU <--\n");
        printf("1. Login to your account\n");
        printf("2. Register a new user\n");
        printf("0. Exit\n");
        printf("\n -> Your choice: ");
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
                    printf("6. Export Bank Statement (.html)\n");
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
                    case 6:
                        exportStatementHTML();
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


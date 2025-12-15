#include "bank.h"

// --> Global variables <--
// for easy access to all functions
User users[MAX_USERS];     // Array with all of users loaded from .txt file
int userCount = 0;         // Number of loaded users
int currentUserIndex = -1; // The Index of the currently loaded user

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
    while (fscanf(fp, "%s %s %s %s %lf %s",
                  users[userCount].firstName,
                  users[userCount].middleName,
                  users[userCount].lastName,
                  users[userCount].accountNumber,
                  &users[userCount].balance,
                  users[userCount].pin) != EOF)
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
        fprintf(fp, "%s %s %s %s %.2f %s\n",
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
int login()
{
    char inputAcc[20];
    char inputPin[7];

    printf("\n--- LOGIN TO THE SYSTEM ---\n");
    printf("Please enter a bank account number: ");
    scanf("%s", inputAcc);
    printf("Please enter a PIN code: ");
    scanf("%s", inputPin);

    // Loop through the user array to find a match
    for (int i = 0; i < userCount; i++)
    {
        // strcmp compare 2 Strings. If return 0 => they are equal.
        if (strcmp(users[i].accountNumber, inputAcc) == 0 && strcmp(users[i].pin, inputPin) == 0)
        {
            currentUserIndex = i; // Remember witch user is enter
            printf("You have successfully logged into your account!\n");
            return 1; // Return true
        }
    }

    return 0; // Return false
}

// Function for verifying the PIN code before bank operation
int verifyPin()
{
    char pinCheck[7];
    printf("For confirmation, please enter your PIN code: ");
    scanf("%s", pinCheck);

    if (strcmp(pinCheck, users[currentUserIndex].pin) == 0)
    {
        return 1; // PIN code is correct (verified)
    }
    else
    {
        printf("Wrong PIN code! Bank operation is denied.\n");
        return 0; // PIN code is wrong
    }
}

// Function for validating the PIN (between 4-6 nums)
// poniter exa
int isValidPin(char *pin)
{
    int len = strlen(pin);

    // length check
    if (len < 4 || len > 6)
    {
        printf("ERROR: PIN must be between 4 nad 6 characters long!\n");
        return 0;
    }

    // Check if all characters are numbers
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(pin[i]))
        {
            printf("ERROR: PIN can only consist of numbers!\n");
            return 0;
        }
    }

    return 1;
}

// Function for register a new user
void registerUser()
{
    if (userCount >= MAX_USERS)
    {
        printf("The maximum number of registered users has been reached!\n");
        printf("Contact the Bank's support department!\n");
        return;
    }

    printf("\n--- New User Registration ---\n");

    // Enter Name
    printf("Enter First Name: ");
    scanf("%s", users[userCount].firstName);
    printf("Enter Middle Name: ");
    scanf("%s", users[userCount].middleName);
    printf("Enter Last Name: ");
    scanf("%s", users[userCount].lastName);

    // Enter PIN
    do
    {
        printf("Create a PIN code (4 to 6 digits): ");
        scanf("%s", users[userCount].pin);
    } while (!isValidPin(users[userCount].pin));

    // Start balance = 0
    users[userCount].balance = 0.00;

    // --- AUTOMATIC ACCOUNT NUMBER GENERATION ---
    int nextId = 1000; // Initial value if data file is empty
    if (userCount > 0)
    {
        // Take the last user's number and convert it from text to a number (atoi)
        int lastId = atoi(users[userCount - 1].accountNumber);
        nextId = lastId + 1;
    }
    // Convert the new number back to text (sprintf)
    sprintf(users[userCount].accountNumber, "%d", nextId);

    printf("\n--- SUCCESSFUL REGISTRATION ---!\n");
    printf("Your account number is: %s\n", users[userCount].accountNumber);
    printf("!REMEMBER IT TO LOGIN TO THE SYSTEM!\n");

    userCount++;       // Increasing the number of people
    saveUsersToFile(); // save the new data file

    return;
}

// Function for check account balance
void showBalance()
{
    printf("\nYour current balance is: %.2f euro.\n", users[currentUserIndex].balance);
}

// Function for money deposit
void depositMoney()
{
    double amount;
    char ch; // variable for testing the input amount

    printf("\nEnter deposit amount: ");

    // TEST 1: THE INPUT IS DIGIT ONLY
    if (scanf("%lf", &amount) != 1)
    {
        printf("ERROR: Please enter a valid number!\n");
        while (getchar() != '\n')
            ; // Clear buffer
        return;
    }
    // scanf("%lf", &amount);

    // TEST 2: THERE ARE NO SYMBOLS AFTER THE DIGIT
    ch = getchar();
    while (ch != '\n' && ch != EOF)
    {
        if (ch != ' ')
        {
            printf("ERROR: Invalid characters after the amount!\n");
            return;
        }
        ch = getchar();
    }

    // TEST 3: THE AMOUNT IS POSITIVE NUMBER
    if (amount <= 0)
    {
        printf("Deposit amount can only be positive number!\n");
        return;
    }

    // Verify PIN code
    if (!verifyPin())
        return;

    users[currentUserIndex].balance += amount;
    saveUsersToFile(); // Save the change to data file
    logTransaction(currentUserIndex, "DEPOSIT", amount); // log the transaction
    printf("You have successfully entered %.2f euro. New balance: %.2f euro.\n", amount, users[currentUserIndex].balance);
}

// Function for money withdraw
void withdrawMoney()
{
    double amount;
    char ch; // variable for testing the input
    printf("\nEnter amount to withdraw: ");

    // TEST 1: THE INPUT IS DIGIT ONLY
    if (scanf("%lf", &amount) != 1)
    {
        printf("ERROR: Please enter a valid number!\n");
        while (getchar() != '\n')
            ; // Clear buffer;
        return;
    }
    // scanf("%lf", &amount);

    // TEST 2: THERE ARE NO SYMBOLS AFTER THE DIGIT
    ch = getchar();
    while (ch != '\n' && ch != EOF)
    {
        if (ch != ' ')
        {
            printf("ERROR: Invalid characters after the amount!\n");
            return;
        }
        ch = getchar();
    }

    // TEST 3: THE AMOUNT IS POSITIVE NUMBER
    if (amount <= 0)
    {
        printf("Withdraw amount can only be positive number!");
        return;
    }

    if (amount > users[currentUserIndex].balance)
    {
        printf("There is no enough money in your account! Your balance is: %.2f euro.\n", users[currentUserIndex].balance);
        return;
    }

    // Verify PIN code
    if (!verifyPin())
        return;

    users[currentUserIndex].balance -= amount;
    saveUsersToFile();
    logTransaction(currentUserIndex, "WITHDRAW", -amount); // log the transaction
    printf("You have successfully withdraw %.2f euro. New balance: %.2f euro.\n", amount, users[currentUserIndex].balance);
}

// Function for transfer money to another bank user
void transferMoney()
{
    char targetAcc[20];
    char targetFirst[50], targetMiddle[50], targetLast[50];
    double amount;
    int targetIndex = -1;

    printf("\n--- Transfer money money to another bank user ---\n");
    printf("Enter recipient's bank account number: ");
    scanf("%s", targetAcc);
    printf("Enter recipient's First Name: ");
    scanf("%s", targetFirst);
    printf("Enter recipient's Middle Name: ");
    scanf("%s", targetMiddle);
    printf("Enter recipient's Last Name: ");
    scanf("%s", targetLast);

    // Search for the recipient in the database
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].accountNumber, targetAcc) == 0 &&
            strcmp(users[i].firstName, targetFirst) == 0 &&
            strcmp(users[i].middleName, targetMiddle) == 0 &&
            strcmp(users[i].lastName, targetLast) == 0)
        {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1)
    {
        printf("ERROR: No such user was found!\n");
        return;
    }

    // Can't transfer money to your account
    if (targetIndex == currentUserIndex)
    {
        printf("Don't try to transfer money to your own account!\n");
        return;
    }

    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    if (amount > users[currentUserIndex].balance)
    {
        printf("You do not have enough money to complete this transfer!\n");
        return;
    }

    // Verify PIN code
    if (!verifyPin())
        return;

    // Make the transfer
    users[currentUserIndex].balance -= amount; // Take money from the sender
    users[targetIndex].balance += amount;      // Transfer money to the recipient

    saveUsersToFile(); // Save new data
    logTransaction(currentUserIndex, "TRANSFER_SENT", -amount); // save lof for sender
    logTransaction(targetIndex, "TRANSFER_RECEIVED", amount); // save log for recipient
    printf("You have successfully transfer %.2f euro to %s %s.\n", amount, users[targetIndex].firstName, users[targetIndex].lastName);
}

// Function for Change PIN code
void changePin()
{
    char newPin[7];

    printf("\n--- Change PIN code ---");

    // Verify the old PIN code
    if (!verifyPin())
        return;

    do
    {
        printf("Enter the new PIN code: ");
        scanf("%s", newPin);
    } while (!isValidPin(newPin));

    strcpy(users[currentUserIndex].pin, newPin);

    saveUsersToFile();
    printf("Your PIN code was changed successfully!\n");
}

// int changeMoney {
// printf ("\n-- CHANGE MONEY --\n");

//}

// Function for make a bank statement for current user and export it to .html file
void exportStatementHTML()
{
    char filename[50];
    sprintf(filename, "statement_%s.html", users[currentUserIndex].accountNumber);

    FILE *fp = fopen(filename, "w"); // Creating HTML file

    if (fp == NULL)
    {
        printf("ERROR while creating HTML file!\n");
        return;
    }

    // HTML file stucture
    fprintf(fp, "<!DOCTYPE html>\n<html>\n<head>\n");

    // Style css
    fprintf(fp, "<style>");
    fprintf(fp, "body { font-family: Arial, sans-serif; background-color: #f4f4f9; text-align: center; }");
    fprintf(fp, ".card { background: white; width: 50%%; margin: 50px auto; padding: 20px; box-shadow: 0 4px 8px rgba(0,0,0,0.2); border-radius: 10px; }");
    fprintf(fp, "h1 { color: #2c3e50; }");
    fprintf(fp, ".balance { font-size: 2em; color: #27ae60; font-weight: bold; }");
    fprintf(fp, "</style>");

    fprintf(fp, "</head>\n<body>\n");

    // Data about the bank user
    fprintf(fp, "<div class='card'>\n");
    fprintf(fp, "  <h1>Bank Statement</h1>\n");
    fprintf(fp, "  <p>Client: <strong>%s %s</strong></p>\n", users[currentUserIndex].firstName, users[currentUserIndex].lastName);
    fprintf(fp, "  <p>Account Number: %s</p>\n", users[currentUserIndex].accountNumber);
    fprintf(fp, "  <hr>\n");
    fprintf(fp, "  <p>Current Balance:</p>\n");
    fprintf(fp, "  <div class='balance'>%.2f EUR</div>\n", users[currentUserIndex].balance);
    fprintf(fp, "  <p><em>Generated by Bank Account Management System </em></p>\n");
    fprintf(fp, "</div>\n");

    fprintf(fp, "</body>\n</html>");

    fclose(fp);
    printf("\n[SUCCESS] Your bank statement is ready!\n");
    printf("Open file '%s' with your browser.\n", filename);
}

// Function for creating .txt file with log transactions history
void logTransaction(int userIndex, const char *type, double amount)
{
    FILE *fp = fopen("transactions.txt", "a"); // append to the end of the .txt file

    if (fp == NULL)
    {
        printf("ERROR while opening transactions log!\n");
        return;
    }

    // Get current time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // ACCOUNT | TYPE | AMOUNT | DATE (DD.MM.YYYY)

    fprintf(fp, "%s | %s | %+.2f | %02d.%02d.%d\n",
            users[userIndex].accountNumber,
            type,
            amount,
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    fclose(fp);
}
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
    while (userCount < MAX_USERS && fscanf(fp, "%s %s %s %s %lf %s",
                  users[userCount].firstName,
                  users[userCount].middleName,
                  users[userCount].lastName,
                  users[userCount].accountNumber,
                  &users[userCount].balance,
                  users[userCount].pin) != EOF)
    {
        caesarCipher(users[userCount].pin, -1); // '-1' -> deciphering
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
        char encryptedPin[7];
        strcpy(encryptedPin, users[i].pin);

        caesarCipher(encryptedPin, 1); // '1' -> ciphering
        fprintf(fp, "%s %s %s %s %.2f %s\n",
                users[i].firstName,
                users[i].middleName,
                users[i].lastName,
                users[i].accountNumber,
                users[i].balance,
                encryptedPin);
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
    saveUsersToFile();                                   // Save the change to data file
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

    saveUsersToFile();                                          // Save new data
    logTransaction(currentUserIndex, "TRANSFER_SENT", -amount); // save lof for sender
    logTransaction(targetIndex, "TRANSFER_RECEIVED", amount);   // save log for recipient
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

// Function to make a bank statement for current user and export it to .html file
void exportStatementHTML()
{

    if (!verifyPin())
        return;

    
    char filename[50];
    sprintf(filename, "statement_%s.html", users[currentUserIndex].accountNumber);

    FILE *fp = fopen(filename, "w");
    FILE *transFile = fopen(LOG_FILENAME, "r"); // Open transaction.txt file

    if (fp == NULL)
    {
        printf("ERROR: Can't run the HTML file!\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char issueDate[20];
    sprintf(issueDate, "%02d.%02d.%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // -> BUILD the HTML file with INTERNAL CSS 
        // --- HTML HEADER & CSS ---
    fprintf(fp, "<!DOCTYPE html>\n<html lang='bg'>\n<head>\n<meta charset='UTF-8'>\n");
    fprintf(fp, "<title>Bank Statement</title>\n");
    fprintf(fp, "<style>\n");
    fprintf(fp, "  body { font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif; background-color: #f4f4f4; color: #333; margin: 0; padding: 40px; }\n");
    fprintf(fp, "  .container { max-width: 800px; margin: 0 auto; background: white; padding: 40px; box-shadow: 0 10px 25px rgba(0,0,0,0.1); border-top: 5px solid #FF8C00; }\n"); 
    fprintf(fp, "  .header { display: flex; justify-content: space-between; align-items: center; border-bottom: 2px solid #eee; padding-bottom: 20px; margin-bottom: 30px; }\n");
    fprintf(fp, "  .logo { font-size: 28px; font-weight: bold; color: #FF8C00; text-transform: uppercase; letter-spacing: 2px; }\n");
    fprintf(fp, "  .bank-details { font-size: 12px; color: #777; text-align: right; }\n");
    fprintf(fp, "  .client-info { background-color: #fff8f0; padding: 20px; border-left: 4px solid #FF8C00; margin-bottom: 30px; }\n"); 
    fprintf(fp, "  .client-info h2 { margin-top: 0; color: #333; font-size: 18px; }\n");
    fprintf(fp, "  .client-info p { margin: 5px 0; font-size: 14px; }\n");

    fprintf(fp, "  table { width: 100%%; border-collapse: collapse; margin-bottom: 40px; }\n");
    fprintf(fp, "  th { background-color: #FF8C00; color: white; text-align: left; padding: 12px; font-size: 14px; }\n");
    fprintf(fp, "  td { padding: 12px; border-bottom: 1px solid #ddd; font-size: 14px; }\n");
    fprintf(fp, "  tr:nth-child(even) { background-color: #f9f9f9; }\n"); 
    fprintf(fp, "  .amount-plus { color: green; font-weight: bold; }\n");
    fprintf(fp, "  .amount-minus { color: red; font-weight: bold; }\n");

    fprintf(fp, "  .footer { display: flex; justify-content: space-between; align-items: flex-end; margin-top: 50px; page-break-inside: avoid; }\n");
    fprintf(fp, "  .stamp-box { position: relative; width: 150px; height: 150px; }\n");
    fprintf(fp, "  .stamp { border: 3px double #FF8C00; color: #FF8C00; border-radius: 50%%; width: 120px; height: 120px; display: flex; align-items: center; justify-content: center; transform: rotate(-15deg); font-weight: bold; text-transform: uppercase; font-size: 14px; text-align: center; opacity: 0.8; }\n");
    fprintf(fp, "  .signature-box { text-align: center; border-top: 1px solid #333; width: 200px; padding-top: 10px; }\n");
    fprintf(fp, "  .small-text { font-size: 12px; color: #999; }\n");
    fprintf(fp, "</style>\n</head>\n<body>\n");

        // --- HTML BODY CONTENT ---

    fprintf(fp, "<div class='container'>\n");

        // 1. Header
    fprintf(fp, "  <div class='header'>\n");
    fprintf(fp, "    <div class='logo'>Dragnev Bank</div>\n");
    fprintf(fp, "    <div class='bank-details'>Dragnev Bank AD<br>Varna, Bulgaria<br>support@dragnevbank.com</div>\n");
    fprintf(fp, "  </div>\n");

        // 2. Client Info Section
    fprintf(fp, "  <div class='client-info'>\n");
    fprintf(fp, "    <h2>BANK STATEMENT</h2>\n");
    fprintf(fp, "    <p><strong>Client:</strong> %s %s %s</p>\n", users[currentUserIndex].firstName, users[currentUserIndex].middleName, users[currentUserIndex].lastName);
    fprintf(fp, "    <p><strong>IBAN/Account:</strong> %s</p>\n", users[currentUserIndex].accountNumber);
    fprintf(fp, "    <p><strong>Current Balance:</strong> %.2f EUR</p>\n", users[currentUserIndex].balance);
    fprintf(fp, "    <p><strong>Date of Issue:</strong> %s</p>\n", issueDate);
    fprintf(fp, "  </div>\n");

        // 3. Transactions Table
    fprintf(fp, "  <h3>Transaction History</h3>\n");
    fprintf(fp, "  <table>\n");
    fprintf(fp, "    <thead>\n");
    fprintf(fp, "      <tr>\n");
    fprintf(fp, "        <th>No.</th>\n");
    fprintf(fp, "        <th>Type</th>\n");
    fprintf(fp, "        <th>Amount</th>\n");
    fprintf(fp, "        <th>Date</th>\n");
    fprintf(fp, "      </tr>\n");
    fprintf(fp, "    </thead>\n");
    fprintf(fp, "    <tbody>\n");

    // Reading log history
    if (transFile != NULL)
    {
        char line[256]; // buffer
        int counter = 1;

        //safely reads each line of transaction file
        while (fgets(line, sizeof(line), transFile)) // line == buffer; siezof(line) == max size;  transFile == source file
        {

            char lineCopy[256];
            strcpy(lineCopy, line); // copy with lineCopy -> strok destroy the main string X

            char *tokenAcc = strtok(lineCopy, "|");

            // while there is still info in .txt file
            if (tokenAcc != NULL && strstr(tokenAcc, users[currentUserIndex].accountNumber) != NULL)
            {

                char *tokenType = strtok(NULL, "|");
                char *tokenAmount = strtok(NULL, "|");
                char *tokenDate = strtok(NULL, "|");

                char amountClass[20] = "amount-plus";
                if (tokenAmount != NULL && strstr(tokenAmount, "-"))
                {
                    strcpy(amountClass, "amount-minus");
                }

                if (tokenType && tokenAmount && tokenDate)
                {
                    fprintf(fp, "      <tr>\n");
                    fprintf(fp, "        <td>%d</td>\n", counter++);
                    fprintf(fp, "        <td>%s</td>\n", tokenType);
                    fprintf(fp, "        <td class='%s'>%s EUR</td>\n", amountClass, tokenAmount);
                    fprintf(fp, "        <td>%s</td>\n", tokenDate);
                    fprintf(fp, "      </tr>\n");
                }
            }
        }
        fclose(transFile);
    }
    else
    {
        fprintf(fp, "<tr><td colspan='4'>No transaction history found.</td></tr>");
    }

    fprintf(fp, "    </tbody>\n");
    fprintf(fp, "  </table>\n");

    // 4. Footer
    fprintf(fp, "  <div class='footer'>\n");

    fprintf(fp, "    <div class='stamp-box'>\n");
    fprintf(fp, "       <div class='stamp'>Dragnev<br>Bank<br>OFFICIAL</div>\n");
    fprintf(fp, "    </div>\n");

    fprintf(fp, "    <div class='signature-box'>\n");
    fprintf(fp, "       <strong>Svetlozar Dragnev</strong><br>\n");
    fprintf(fp, "       <span class='small-text'>Chief Financial Officer</span>\n");
    fprintf(fp, "    </div>\n");

    fprintf(fp, "  </div>\n"); // End footer
    fprintf(fp, "</div>\n");   // End container

    fprintf(fp, "</body>\n</html>");
    fclose(fp);

    printf("\n[SUCCESS] Your bank statement is ready!!\n");
    printf("Open the file '%s' in your browser.\n", filename);

    // invoke function to open .html file
    openFile(filename);
}

// Function for creating .txt file with log transactions history
void logTransaction(int userIndex, const char *type, double amount)
{
    FILE *fp = fopen(LOG_FILENAME, "a"); // append to the end of the .txt file

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

// Function for ciphering the PIN codes in accounts.txt
// Using CAESAR CIPHER (task from cs50)
void caesarCipher(char *pin, int mode)
{
    int len = strlen(pin);
    int shift = CAESAR_KEY;

    for (int i = 0; i < len; i++)
    {
        // TEST if it is a digit
        if (isdigit(pin[i]))
        {
            // - char to int ('0'-'9' -> 0-9)
            int digit = pin[i] - '0';

            if (mode == 1)
            { // ciphering
                digit = (digit + shift) % 10;
            }
            else if (mode == -1)
            { // deciphering
                digit = (digit - shift);
                if (digit < 0)
                    digit += 10; // If result is negative num
            }

            // int to char (0 - 9 -> '0'-'9')
            pin[i] = digit + '0';
        }
    }
}

// Function to open the already created bank statement
// The .html file opens in default browser
int openFile(char *filename)
{

#define PROG "start" //macros -> replace 'PROG' with "start"
// "start" command searches for an existing file with the given name and opens it in the default app.

    long sizeof_prog = strlen(filename) + strlen(PROG) + 2;  //calculate memory => fileNameLength + "start" + space + \0
    
    char *program = calloc(sizeof_prog, 1); // allocates dynamicly memory and zeroes it out
                                            // (1 => 1 byte per char)
    snprintf(program, sizeof_prog, "%s %s", PROG, filename); //assembles the command => result: "start .\filename.html"

    int rc = system(program); // execute program with it's argument ("start .\filename.html")

    //TEST if result code is != 0
    if (rc)
    {
        printf("Command execution failed with %d\n", rc);
        return rc;
    }
    return 0;
}
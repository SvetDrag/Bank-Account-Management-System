# 🏦 Bank Account Management System

> A secure, console-based banking application developed in C with HTML export features.

![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![IDE](https://img.shields.io/badge/IDE-VS_Code-blueviolet?style=for-the-badge&logo=visual-studio-code&logoColor=white)
![Course](https://img.shields.io/badge/Course-Programming_Basics_and_Algorithmization-green?style=for-the-badge)


## 📖 Overview
This project was developed as a university course assignment for the discipline **"Programming Basics and Algorithmization"** at the Nikola Vaptsarov Naval Academy.

The software simulates a core banking system (ATM logic), allowing users to manage accounts, perform transactions, and generate financial reports. It focuses on modular architecture, data persistence, and basic security concepts.

* **Developer:** Svetlozar D. Dragnev
* **Student ID:** 1224-25120
* **Specialty:** ICT (1st Year)
* **Development Period:** November - December 2025

---

### 📷 Screenshots
---

## ✨ Key Features
* **User Management:** Registration system with automatic IBAN generation.
* **🔐 Secure Authentication:** Login system using Account Number & PIN.
* **🛡️ Data Encryption:** Implements **Caesar Cipher** algorithm to encrypt sensitive data (PINs) in the database.
* **💰 Financial Operations:**
    * Deposit & Withdraw funds.
    * Transfer money to other users.
    * Real-time Balance check.
* **📜 Transaction Logging:** All actions are recorded in `transactions.txt` with accurate timestamps.
* **📊 HTML Reporting:** Generates a professional **Bank Statement (.html)** with CSS styling.
* **✅ Input Validation:** Robust error handling to prevent crashes (buffer clearing).

## 🛠️ Tech Stack & Libraries
The project is built using **C** for the backend logic and **HTML/CSS** for document generation.

| Library | Usage Description |
| :--- | :--- |
| `<stdio.h>` | Standard input/output operations (`printf`, `scanf`, file handling). |
| `<stdlib.h>` | Memory management, conversions (`atoi`) and process control. |
| `<string.h>` | String manipulation (`strcmp`, `strcpy`, `strtok` for parsing data). |
| `<ctype.h>` | Character validation (checking if PIN contains only digits). |
| `<time.h>` | timestamp generation for transaction logs and bank statements. |

## 📂 Project Structure
```text
Bank-Account-Management-System/
├── 📄 main_bank.c          # Entry point (Main menu loop)
├── 📄 bank_functions.c     # Core logic (Login, Encryption, I/O)
├── 📄 bank.h               # Header file (Structures & Prototypes)
├── 📁 data/                # Data storage (Optional organization)
│   ├── 🔒 accounts.txt     # Encrypted user database
│   └── 📜 transactions.txt # Transaction history log
├── 📄 README.md            # Project documentation
└── 📄 bank_app.exe         # Compiled executable
```

## 🚀 How to Run
Prerequisites: A C compiler (GCC recommended).

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/SvetDrag/Bank-Account-Management-System.git
    cd Bank-Account-Management-System
    ```
2.  **Compile the source files together:**
    ```bash
    gcc main_bank.c bank_functions.c -o bank_app
    ```
3.  **Run the application:**
    ```bash
    ./bank_app
    ```

## 🔮 Future Development
To make the system sustainable and enterprise-ready, the following improvements are planned:
* [ ] GUI:** Implementing a visual interface (using C# or Python wrapper).
* [ ] Database:** Migrating from `.txt` files to SQL (SQLite/PostgreSQL) for better scalability.
* [ ] Security:** Improving encryption (Hashing/Salt) and adding 2FA (Email/SMS).
* [ ] Features:** Multi-currency wallets and loan calculator.

## 💡 Resources and Knowledge
* **Lectures on Programming Basics and Algorithmization (NVNA);**
* **CS50 Harvard Course;**
* **The C Programming Language. 2nd Edition** Brian W. Kernighan & Dennis M. Ritchie (BG version);
* **Other:** YouTube videos, W3School, GeeksForGeeks, AI assistant.

---
Email: [122325120@naval-acad.bg](mailto:122325120@naval-acad.bg) | [svetlozardragnev@gmail.com](mailto:svetlozardragnev@gmail.com)

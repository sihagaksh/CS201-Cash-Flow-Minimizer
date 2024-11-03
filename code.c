#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include<math.h>

#define MAX_USERS 100

typedef struct {
    int id;
    char name[50];
    double balance;
} User;

typedef struct {
    int id;
    char description[100];
    double amount;
    int paidBy;
    int splitBetween[MAX_USERS];
    int splitCount;
} Expense;

User users[MAX_USERS];
int userCount = 0;

Expense expenses[MAX_USERS];
int expenseCount = 0;

void handleUserInput(){
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            addUser();
            break;
        case 2:
            addExpense();
            break;
        case 3:
            printBalances();
            break;
        case 4:
            minimizeCashFlow();
            break;
        case 5:
            printf("Exiting the system. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
    }
}

void printMenu(){
    printf("\nMenu:\n");
    printf("1. Add User\n");
    printf("2. Add Expense\n");
    printf("3. Print Balances\n");
    printf("4. Minimize Cash Flow\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void addUser(){
    char name[50];
    printf("Enter the name of the user:- ");
    scanf("%s", name);
    userCount++;
    users[userCount].id = userCount + 1;
    strcpy(users[userCount].name, name);
    users[userCount].balance = 0;
    printf("User %s added successfully with ID %d\n", name, users[userCount - 1].id);
}

int main(){
    while(1){
        printMenu();
        handleUserInput();
    }
}

void addExpense(){
    char description[100];
    double amount;
    int paidBy, splitCount;
    int splitBetween[MAX_USERS];

    printf("Enter the description of the expense: ");
    scanf("%s", description);
    printf("Enter the amount paid:- ");
    scanf("%lf", &amount);

    printUsers();
    printf("Enter the ID of the person who paid: ");
    scanf("%d", &paidBy);

    printf("Enter the number of users splitting the expense: ");
    scanf("%d", &splitCount);

    if (splitCount == userCount) {
        for (int i = 0; i < userCount; i++) {
            splitBetween[i] = users[i].id;  
        }
    } else {
        printf("Enter the IDs of the users splitting the expense (separated by space): ");
        for (int i = 0; i < splitCount; i++) {
            scanf("%d", &splitBetween[i]);
        }
    }

    expenses[expenseCount].id = expenseCount + 1;
    strcpy(expenses[expenseCount].description, description);
    expenses[expenseCount].amount = amount;
    expenses[expenseCount].paidBy = paidBy;
    expenses[expenseCount].splitCount = splitCount;
    
    for (int i = 0; i < splitCount; i++) {
        expenses[expenseCount].splitBetween[i] = splitBetween[i];
    }

    double splitAmount = amount / splitCount;

    for (int i = 0; i < splitCount; i++) {
        users[splitBetween[i] - 1].balance -= splitAmount;
    }
    users[paidBy - 1].balance += amount;
    expenseCount++;
    printf("Expense added successfully: %s - Amount: %.2f paid by %s\n", description, amount, users[paidBy - 1].name);
}

void printUsers(){
    printf("List of users:\n");
    for (int i = 0; i < userCount; i++) {
        printf("Id: %d, Name: %s\n", users[i].id, users[i].name);
    }
}
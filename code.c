#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include<math.h>

#define MAX_USERS 100
#define EPSILON 1e-2

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

typedef struct {
    double balance;
    int index;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_USERS];
    int size;
} MaxHeap, MinHeap;

User users[MAX_USERS];
int userCount = 0;

Expense expenses[MAX_USERS];
int expenseCount = 0;

void printMenu();
void handleUserInput();
void printUsers();
void printBalance();
void addUser();
void addExpense();
void minimizeCashFlow(); // Add this line
void insertMaxHeap(MaxHeap* heap, double balance, int index);
void insertMinHeap(MinHeap* heap, double balance, int index);
HeapNode extractMax(MaxHeap* heap);
HeapNode extractMin(MinHeap* heap);
void heapifyUpMax(MaxHeap* heap, int idx);
void heapifyUpMin(MinHeap* heap, int idx);
void heapifyDownMax(MaxHeap* heap, int idx);
void heapifyDownMin(MinHeap* heap, int idx);

void addUser(){
    char name[50];
    printf("Enter the name of the user:- ");
    scanf("%s", name);
    users[userCount].id = userCount + 1;
    strcpy(users[userCount].name, name);
    users[userCount].balance = 0;
    userCount++;
    printf("User %s added successfully with ID %d\n", name, users[userCount - 1].id);
}

void printUsers(){
    printf("List of users:\n");
    for (int i = 0; i < userCount; i++) {
        printf("ID: %d, Name: %s\n", users[i].id, users[i].name);
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

void printBalance(){
    printf("\nUser Balances:\n");
    for (int i = 0; i < userCount; i++) {
        printf("%s: %.2f\n", users[i].name, users[i].balance);
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
            printBalance();
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

// Helper functions for heap operations
void insertMaxHeap(MaxHeap* heap, double balance, int index) {
    heap->nodes[heap->size].balance = balance;
    heap->nodes[heap->size].index = index;
    heapifyUpMax(heap, heap->size);
    heap->size++;
}

void insertMinHeap(MinHeap* heap, double balance, int index) {
    heap->nodes[heap->size].balance = balance;
    heap->nodes[heap->size].index = index;
    heapifyUpMin(heap, heap->size);
    heap->size++;
}

HeapNode extractMax(MaxHeap* heap) {
    HeapNode maxNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    heapifyDownMax(heap, 0);
    return maxNode;
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode minNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    heapifyDownMin(heap, 0);
    return minNode;
}

void heapifyUpMax(MaxHeap* heap, int idx) {
    while (idx > 0 && heap->nodes[(idx - 1) / 2].balance < heap->nodes[idx].balance) {
        HeapNode temp = heap->nodes[idx];
        heap->nodes[idx] = heap->nodes[(idx - 1) / 2];
        heap->nodes[(idx - 1) / 2] = temp;
        idx = (idx - 1) / 2;
    }
}

void heapifyUpMin(MinHeap* heap, int idx) {
    while (idx > 0 && heap->nodes[(idx - 1) / 2].balance > heap->nodes[idx].balance) {
        HeapNode temp = heap->nodes[idx];
        heap->nodes[idx] = heap->nodes[(idx - 1) / 2];
        heap->nodes[(idx - 1) / 2] = temp;
        idx = (idx - 1) / 2;
    }
}

void heapifyDownMax(MaxHeap* heap, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].balance > heap->nodes[largest].balance) {
        largest = left;
    }
    if (right < heap->size && heap->nodes[right].balance > heap->nodes[largest].balance) {
        largest = right;
    }
    if (largest != idx) {
        HeapNode temp = heap->nodes[idx];
        heap->nodes[idx] = heap->nodes[largest];
        heap->nodes[largest] = temp;
        heapifyDownMax(heap, largest);
    }
}

void heapifyDownMin(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].balance < heap->nodes[smallest].balance) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].balance < heap->nodes[smallest].balance) {
        smallest = right;
    }
    if (smallest != idx) {
        HeapNode temp = heap->nodes[idx];
        heap->nodes[idx] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        heapifyDownMin(heap, smallest);
    }
}


void minimizeCashFlow() {
    MaxHeap creditors = { .size = 0 };
    MinHeap debtors = { .size = 0 };

    for (int i = 0; i < userCount; i++) {
        if (fabs(users[i].balance) > EPSILON) {
            if (users[i].balance > 0) {
                insertMaxHeap(&creditors, users[i].balance, i);
            } else {
                insertMinHeap(&debtors, users[i].balance, i);
            }
        }
    }

    while (creditors.size > 0 && debtors.size > 0) {
        HeapNode maxCreditor = extractMax(&creditors);
        HeapNode maxDebtor = extractMin(&debtors);

        double settlementAmount = fmin(maxCreditor.balance, -maxDebtor.balance);
        users[maxCreditor.index].balance -= settlementAmount;
        users[maxDebtor.index].balance += settlementAmount;

        printf("%s pays %.2f to %s\n", users[maxDebtor.index].name, settlementAmount, users[maxCreditor.index].name);

        if (users[maxCreditor.index].balance > EPSILON) {
            insertMaxHeap(&creditors, users[maxCreditor.index].balance, maxCreditor.index);
        }
        if (users[maxDebtor.index].balance < -EPSILON) {
            insertMinHeap(&debtors, users[maxDebtor.index].balance, maxDebtor.index);
        }
    }

    printf("All transactions settled.\n");
}

int main(){
    while(1){
        printMenu();
        handleUserInput();
    }
}
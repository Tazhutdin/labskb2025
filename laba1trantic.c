#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>   // ��� �����/������ (printf, scanf)
#include <stdlib.h>  // ��� ������ � ������� (FILE, fopen, fclose)
#include <string.h>  // ��� ������ �� �������� (strcpy, strcmp)
#include <locale.h>

#define MAX_EXPENSES 1000  // ������������ ���������� ��������
#define MAX_CATEGORY_LEN 30
#define MAX_DATE_LEN 11

// ��������� ��� ������ �������
typedef struct {
    char date[MAX_DATE_LEN];        // ���� � ������� "��.��.����"
    char category[MAX_CATEGORY_LEN]; // ��������� �������
    float amount;                   // ����� �������
} Expense;

// ���������� ������ ��� �������� ���� ��������
Expense expenses[MAX_EXPENSES];
int expenseCount = 0; // ������� ���������� �������

// --- ������� ��� �������� �������� �� ����� ---
void loadExpenses(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return; // ���� ���� �� ������ � �������
    }

    // ������ ������ ���� ���� ��� ������
    while (fscanf(file, "%10s %29s %f",
        expenses[expenseCount].date,
        expenses[expenseCount].category,
        &expenses[expenseCount].amount) == 3) {
        expenseCount++;
    }

    fclose(file);
}

// --- ������� ��� ���������� ������ ������� � ���� ---
void saveExpenseToFile(const char* filename, Expense e) {
    FILE* file = fopen(filename, "a"); // ����� ����������

    if (!file) {
        printf("������ ��� ���������� �����!\n");
        return;
    }

    fprintf(file, "%s %s %.2f\n", e.date, e.category, e.amount);
    fclose(file);
}

// --- ������� ��� ���������� ������ ������� ---
void addExpense() {
    // ��������� �����
    if (expenseCount >= MAX_EXPENSES) {
        printf("�������� ����� ��������!\n");
        return;
    }

    Expense e;

    printf("������� ���� (��.��.����): ");
    scanf("%10s", e.date);

    printf("������� ���������: ");
    scanf("%29s", e.category);

    printf("������� �����: ");
    scanf("%f", &e.amount);

    // ��������� � ������ � ��������� � ����
    expenses[expenseCount++] = e;
    saveExpenseToFile("expenses.txt", e);

    printf("������ ��������!\n");
}

// --- ������� ��� ����������� ���� �������� ---
void showAllExpenses() {
    if (expenseCount == 0) {
        printf("��� ������ � ��������.\n");
        return;
    }

    printf("\n--- ��� ������� ---\n");
    printf("%-12s %-15s %s\n", "����", "���������", "�����");
    printf("------------------------------\n");

    for (int i = 0; i < expenseCount; i++) {
        printf("%-12s %-15s %.2f ���.\n",
            expenses[i].date,
            expenses[i].category,
            expenses[i].amount);
    }
}

// --- ������� ��� ������ ���������� ---
void showStats() {
    if (expenseCount == 0) {
        printf("��� ������ ��� �������.\n");
        return;
    }

    float total = 0;
    float max = expenses[0].amount;

    for (int i = 0; i < expenseCount; i++) {
        total += expenses[i].amount;

        if (expenses[i].amount > max) {
            max = expenses[i].amount;
        }
    }

    float average = total / expenseCount;

    printf("\n--- ���������� ---\n");
    printf("����� ���������: %.2f ���.\n", total);
    printf("������� �����: %.2f ���.\n", average);
    printf("������������ �����: %.2f ���.\n", max);
}

// --- ������� ��� ����������� ���� ������������ ---
void showMenu() {
    printf("\n=== ������ �������� ===\n");
    printf("1. �������� ������\n");
    printf("2. �������� ��� �������\n");
    printf("3. �������� ����������\n");
    printf("4. �����\n");
    printf("�������� �����: ");
}

// --- ������� ������� ��������� ---
int main() {
    setlocale(LC_ALL, "Russian");

    loadExpenses("expenses.txt"); // ��������� ������ �� �����
   

    int choice;

    do {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addExpense();
            break;
        case 2:
            showAllExpenses();
            break;
        case 3:
            showStats();
            break;
        case 4:
            printf("����� �� ���������.\n");
            break;
        default:
            printf("�������� �����.\n");
            // ������� ������ ����� ��� �������� ������
            while (getchar() != '\n');
        }

    } while (choice != 4);

    return 0;
}
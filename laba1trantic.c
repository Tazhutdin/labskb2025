#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>   // Для ввода/вывода (printf, scanf)
#include <stdlib.h>  // Для работы с файлами (FILE, fopen, fclose)
#include <string.h>  // Для работы со строками (strcpy, strcmp)
#include <locale.h>

#define MAX_EXPENSES 1000  // Максимальное количество расходов
#define MAX_CATEGORY_LEN 30
#define MAX_DATE_LEN 11

// Структура для одного расхода
typedef struct {
    char date[MAX_DATE_LEN];        // Дата в формате "дд.мм.гггг"
    char category[MAX_CATEGORY_LEN]; // Категория расхода
    float amount;                   // Сумма расхода
} Expense;

// Глобальный массив для хранения всех расходов
Expense expenses[MAX_EXPENSES];
int expenseCount = 0; // Текущее количество записей

// --- Функция для загрузки расходов из файла ---
void loadExpenses(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return; // Если файл не найден — выходим
    }

    // Чтение данных пока есть что читать
    while (fscanf(file, "%10s %29s %f",
        expenses[expenseCount].date,
        expenses[expenseCount].category,
        &expenses[expenseCount].amount) == 3) {
        expenseCount++;
    }

    fclose(file);
}

// --- Функция для сохранения одного расхода в файл ---
void saveExpenseToFile(const char* filename, Expense e) {
    FILE* file = fopen(filename, "a"); // Режим добавления

    if (!file) {
        printf("Ошибка при сохранении файла!\n");
        return;
    }

    fprintf(file, "%s %s %.2f\n", e.date, e.category, e.amount);
    fclose(file);
}

// --- Функция для добавления нового расхода ---
void addExpense() {
    // Проверяем лимит
    if (expenseCount >= MAX_EXPENSES) {
        printf("Превышен лимит расходов!\n");
        return;
    }

    Expense e;

    printf("Введите дату (дд.мм.гггг): ");
    scanf("%10s", e.date);

    printf("Введите категорию: ");
    scanf("%29s", e.category);

    printf("Введите сумму: ");
    scanf("%f", &e.amount);

    // Добавляем в массив и сохраняем в файл
    expenses[expenseCount++] = e;
    saveExpenseToFile("expenses.txt", e);

    printf("Расход добавлен!\n");
}

// --- Функция для отображения всех расходов ---
void showAllExpenses() {
    if (expenseCount == 0) {
        printf("Нет данных о расходах.\n");
        return;
    }

    printf("\n--- Все расходы ---\n");
    printf("%-12s %-15s %s\n", "Дата", "Категория", "Сумма");
    printf("------------------------------\n");

    for (int i = 0; i < expenseCount; i++) {
        printf("%-12s %-15s %.2f руб.\n",
            expenses[i].date,
            expenses[i].category,
            expenses[i].amount);
    }
}

// --- Функция для вывода статистики ---
void showStats() {
    if (expenseCount == 0) {
        printf("Нет данных для анализа.\n");
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

    printf("\n--- Статистика ---\n");
    printf("Всего потрачено: %.2f руб.\n", total);
    printf("Средняя трата: %.2f руб.\n", average);
    printf("Максимальная трата: %.2f руб.\n", max);
}

// --- Функция для отображения меню пользователя ---
void showMenu() {
    printf("\n=== Трекер расходов ===\n");
    printf("1. Добавить расход\n");
    printf("2. Показать все расходы\n");
    printf("3. Показать статистику\n");
    printf("4. Выход\n");
    printf("Выберите пункт: ");
}

// --- Главная функция программы ---
int main() {
    setlocale(LC_ALL, "Russian");

    loadExpenses("expenses.txt"); // Загружаем данные из файла
   

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
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор.\n");
            // Очистка буфера ввода при неверном выборе
            while (getchar() != '\n');
        }

    } while (choice != 4);

    return 0;
}
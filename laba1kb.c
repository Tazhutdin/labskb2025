#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h> 

#define MAX_BOOKS 1000
#define MAX_TITLE 100
#define MAX_AUTHOR 50

// Структура для хранения информации о книге
struct Book {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int year;
    float rating;
};

// Глобальные переменные
struct Book books[MAX_BOOKS];
int bookCount = 0;
const char* filename = "books.txt";

// Функция для загрузки книг из файла
void loadBooks() {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Файл %s не найден. Будет создан новый при сохранении.\n", filename);
        return;
    }

    bookCount = 0;
    while (bookCount < MAX_BOOKS) {
        // Для fscanf_s нужно указывать размеры буферов
        int result = fscanf_s(file, "%99[^|]|%49[^|]|%d|%f\n",
            books[bookCount].title, (unsigned)sizeof(books[bookCount].title),
            books[bookCount].author, (unsigned)sizeof(books[bookCount].author),
            &books[bookCount].year,
            &books[bookCount].rating);

        if (result == 4) {
            bookCount++;
        }
        else {
            break; // Прерываем цикл если не удалось прочитать
        }
    }
    fclose(file);
    printf("Загружено книг: %d\n", bookCount);
}

// Функция для сохранения книг в файл
void saveBooks() {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка создания файла!\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "%s|%s|%d|%.1f\n",
            books[i].title,
            books[i].author,
            books[i].year,
            books[i].rating);
    }

    fclose(file);
    printf("Данные сохранены в %s\n", filename);
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Функция для добавления книги
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Достигнут лимит книг (%d)!\n", MAX_BOOKS);
        return;
    }

    struct Book newBook;

    printf("=== Добавление новой книги ===\n");


    clearInputBuffer();

    printf("Введите название книги: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Удаляем символ новой строки

    printf("Введите автора: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("Год прочтения: ");
    scanf_s("%d", &newBook.year);

    printf("Ваша оценка (0-10): ");
    scanf_s("%f", &newBook.rating);

    // Проверка оценки
    if (newBook.rating < 0 || newBook.rating > 10) {
        printf("Оценка должна быть от 0 до 10!\n");
        return;
    }

    books[bookCount] = newBook;
    bookCount++;

    printf("Книга добавлена!\n");
}

// Функция для показа всех книг
void showAllBooks() {
    if (bookCount == 0) {
        printf("Список книг пуст.\n");
        return;
    }

    printf("\n=== Список всех книг ===\n");
    printf("%-40s %-25s %-6s %-5s\n", "Название", "Автор", "Год", "Оценка");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < bookCount; i++) {
        printf("%-40s %-25s %-6d %-5.1f\n",
            books[i].title,
            books[i].author,
            books[i].year,
            books[i].rating);
    }
    printf("\n");
}

// Функция для показа статистики
void showStats() {
    if (bookCount == 0) {
        printf("Нет данных для статистики.\n");
        return;
    }

    float totalRating = 0;
    float maxRating = -1;
    int bestBookIndex = -1;

    for (int i = 0; i < bookCount; i++) {
        totalRating += books[i].rating;

        if (books[i].rating > maxRating) {
            maxRating = books[i].rating;
            bestBookIndex = i;
        }
    }

    printf("\n=== Статистика ===\n");
    printf("Количество прочитанных книг: %d\n", bookCount);
    printf("Средняя оценка: %.2f\n", totalRating / bookCount);

    if (bestBookIndex != -1) {
        printf("Самая высоко оценённая книга:\n");
        printf("  \"%s\" - %s (Оценка: %.1f)\n",
            books[bestBookIndex].title,
            books[bestBookIndex].author,
            books[bestBookIndex].rating);
    }
}

// Главное меню
void showMenu() {
    printf("\n=== Трекер книг ===\n");
    printf("1. Добавить книгу\n");
    printf("2. Показать все книги\n");
    printf("3. Показать статистику\n");
    printf("4. Сохранить и выйти\n");
    printf("Выберите пункт: ");
}

// Основная функция
int main() {
    SetConsoleCP(1251);        // Установка кодовой страницы ввода
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    int choice;


    printf("Добро пожаловать в Трекер книг!\n");
    loadBooks(); // Загружаем книги при запуске

    do {
        showMenu();
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            showAllBooks();
            break;
        case 3:
            showStats();
            break;
        case 4:
            saveBooks();
            printf("Данные сохранены. До свидания!\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 4);

    return 0;
}
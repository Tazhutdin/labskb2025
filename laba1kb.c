#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h> 

#define MAX_BOOKS 1000
#define MAX_TITLE 100
#define MAX_AUTHOR 50

// ��������� ��� �������� ���������� � �����
struct Book {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int year;
    float rating;
};

// ���������� ����������
struct Book books[MAX_BOOKS];
int bookCount = 0;
const char* filename = "books.txt";

// ������� ��� �������� ���� �� �����
void loadBooks() {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("���� %s �� ������. ����� ������ ����� ��� ����������.\n", filename);
        return;
    }

    bookCount = 0;
    while (bookCount < MAX_BOOKS) {
        // ��� fscanf_s ����� ��������� ������� �������
        int result = fscanf_s(file, "%99[^|]|%49[^|]|%d|%f\n",
            books[bookCount].title, (unsigned)sizeof(books[bookCount].title),
            books[bookCount].author, (unsigned)sizeof(books[bookCount].author),
            &books[bookCount].year,
            &books[bookCount].rating);

        if (result == 4) {
            bookCount++;
        }
        else {
            break; // ��������� ���� ���� �� ������� ���������
        }
    }
    fclose(file);
    printf("��������� ����: %d\n", bookCount);
}

// ������� ��� ���������� ���� � ����
void saveBooks() {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������ �������� �����!\n");
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
    printf("������ ��������� � %s\n", filename);
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ������� ��� ���������� �����
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("��������� ����� ���� (%d)!\n", MAX_BOOKS);
        return;
    }

    struct Book newBook;

    printf("=== ���������� ����� ����� ===\n");


    clearInputBuffer();

    printf("������� �������� �����: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // ������� ������ ����� ������

    printf("������� ������: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("��� ���������: ");
    scanf_s("%d", &newBook.year);

    printf("���� ������ (0-10): ");
    scanf_s("%f", &newBook.rating);

    // �������� ������
    if (newBook.rating < 0 || newBook.rating > 10) {
        printf("������ ������ ���� �� 0 �� 10!\n");
        return;
    }

    books[bookCount] = newBook;
    bookCount++;

    printf("����� ���������!\n");
}

// ������� ��� ������ ���� ����
void showAllBooks() {
    if (bookCount == 0) {
        printf("������ ���� ����.\n");
        return;
    }

    printf("\n=== ������ ���� ���� ===\n");
    printf("%-40s %-25s %-6s %-5s\n", "��������", "�����", "���", "������");
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

// ������� ��� ������ ����������
void showStats() {
    if (bookCount == 0) {
        printf("��� ������ ��� ����������.\n");
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

    printf("\n=== ���������� ===\n");
    printf("���������� ����������� ����: %d\n", bookCount);
    printf("������� ������: %.2f\n", totalRating / bookCount);

    if (bestBookIndex != -1) {
        printf("����� ������ �������� �����:\n");
        printf("  \"%s\" - %s (������: %.1f)\n",
            books[bestBookIndex].title,
            books[bestBookIndex].author,
            books[bestBookIndex].rating);
    }
}

// ������� ����
void showMenu() {
    printf("\n=== ������ ���� ===\n");
    printf("1. �������� �����\n");
    printf("2. �������� ��� �����\n");
    printf("3. �������� ����������\n");
    printf("4. ��������� � �����\n");
    printf("�������� �����: ");
}

// �������� �������
int main() {
    SetConsoleCP(1251);        // ��������� ������� �������� �����
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    int choice;


    printf("����� ���������� � ������ ����!\n");
    loadBooks(); // ��������� ����� ��� �������

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
            printf("������ ���������. �� ��������!\n");
            break;
        default:
            printf("�������� �����. ���������� �����.\n");
        }
    } while (choice != 4);

    return 0;
}
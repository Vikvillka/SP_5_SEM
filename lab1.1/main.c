#define _CRT_SECURE_NO_WARNINGS
#include "header.h";

int main() {
    setlocale(LC_ALL, "Rus");

    char text[] = "This is a test string.";
    printf("\tFirst text (array):\n%s\n", text);
    int error;

    int wordCount = CountWordsArray(text, &error);
    if (error == 0) {
        printf("Number of words in text: %d\n", wordCount);
    }
    else {
        printf("Error: %d\n", error);
    }

    const char* textPointer = "This is a test string. Hello world!";
    printf("\n\tSecond text (pointer):\n%s\n", textPointer);
    wordCount = CountWordsPointer(textPointer, &error);
    if (error == 0) {
        printf("Number of words in textPointer: %d\n", wordCount);
    }
    else {
        printf("Error: %d\n", error);
    }

    // Тестирование 
    printf("\nTesting\n");
    wordCount = CountWordsArray(NULL, &error);
    if (error != 0) {
        printf("Test with NULL pointer (Array): Error: %d\n", error);
    }

    wordCount = CountWordsPointer(NULL, &error);
    if (error != 0) {
        printf("Test with NULL pointer (Pointer): Error: %d\n", error);
    }

    wordCount = CountWordsArray("", &error);
    if (error == 0) {
        printf("Test with empty string (Array): Number of words: %d\n", wordCount);
    }
    else {
        printf("Test with empty string (Array): Error: %d\n", error);
    }

    wordCount = CountWordsPointer("", &error);
    if (error == 0) {
        printf("Test with empty string (Pointer): Number of words: %d\n", wordCount);
    }
    else {
        printf("Test with empty string (Pointer): Error: %d\n", error);
    }

    return 0;
}
#include "header.h"

int CountWordsArray(char s[], int* error) {
    if (s == NULL) {
        *error = ERROR_NULL_POINTER; 
        return -1; 
    }

    int count = 0;
    int inWord = 0;
    int length = strlen(s);

    if (length == 0) {
        *error = ERROR_EMPTY_STRING; 
        return 0; 
    }

    for (int i = 0; i < length; i++) {
        if (isspace(s[i])) {
            inWord = 0; 
        }
        else if (inWord == 0) {
            inWord = 1; 
            count++;    
        }
    }

    *error = 0; 
    return count;
}

int CountWordsPointer(const char* s, int* error) {
    if (s == NULL) {
        *error = ERROR_NULL_POINTER;
        return -1; 
    }

    int count = 0;
    int inWord = 0;

    while (*s) {
        if (isspace(*s)) {
            inWord = 0;
        }
        else if (inWord == 0) {
            inWord = 1; 
            count++;   
        }
        s++; 
    }

    *error = 0; 
    return count;
}

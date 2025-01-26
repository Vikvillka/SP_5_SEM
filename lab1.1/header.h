#pragma once
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define ERROR_NULL_POINTER -1
#define ERROR_EMPTY_STRING -2

int CountWordsArray(char s[], int* error);
int CountWordsPointer(char *s, int* error);
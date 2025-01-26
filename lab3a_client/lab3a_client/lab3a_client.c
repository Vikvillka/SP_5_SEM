#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#define BUFFER_SIZE 4096

int is_prime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "");
    fwide(stderr, 1);

    if (argc < 3) {
        fwprintf(stderr, L"Использование: %s <нижняя граница> <верхняя граница> [имя мьютекса]\n", argv[0]);
        return 1;
    }

    int lower_bound = _wtoi(argv[1]);
    int upper_bound = _wtoi(argv[2]);
    wchar_t* mutex_name = (argc > 3) ? argv[3] : (wchar_t*)L"Global\\DefaultMutex";

    HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, mutex_name);
    if (hMutex == NULL) {
        fwprintf(stderr, L"Ошибка открытия мьютекса:\n");
        return 1;
    }

    WaitForSingleObject(hMutex, INFINITE);

    HANDLE hPipe = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hPipe == INVALID_HANDLE_VALUE) {
        fwprintf(stderr, L"Ошибка открытия pipe\n");
        CloseHandle(hMutex);
        return 1;
    }


    wchar_t buffer[BUFFER_SIZE];
    int buffer_index = 0;


    for (int i = lower_bound; i <= upper_bound; i++) {
        if (is_prime(i)) {
            buffer_index += _snwprintf(buffer + buffer_index, BUFFER_SIZE - buffer_index, L"%d ", i);
        }
    }

    DWORD bytes_written;
    WriteFile(hPipe, buffer, buffer_index * 2, &bytes_written, NULL);

    ReleaseMutex(hMutex);

    CloseHandle(hMutex);
    return 0;
}

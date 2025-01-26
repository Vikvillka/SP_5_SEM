#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <wchar.h>

#define DEFAULT_PIPE_NAME L"\\\\.\\pipe\\myPipe"
#define BUFFER_SIZE 512


int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "");

    fwide(stderr, 1);

    if (argc < 2) {
        fwprintf(stderr, L"Использование <строка> [имя канала] [имя семафора]");
        return EXIT_FAILURE;
    }

    const wchar_t* inputString = argv[1];
    const wchar_t* pipeName = (argc > 2) ? argv[2] : DEFAULT_PIPE_NAME;

    HANDLE hPipe;
    wchar_t buffer[BUFFER_SIZE];
    DWORD bytesRead, bytesWritten;
    BOOL result;

    hPipe = CreateFileW(
        DEFAULT_PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        wprintf(L"Ошибка подключения к каналу\n");
        return 1;
    }

    wprintf(L"Подключено к серверу\n");

    Sleep(5000);
    wcscpy(buffer, inputString);
    result = WriteFile(hPipe, buffer, wcslen(buffer) * 2 + 1, &bytesWritten, NULL);
    if (!result) {
        wprintf(L"Ошибка отправки данных\n");
        CloseHandle(hPipe);
        return 1;
    }

    result = ReadFile(hPipe, buffer, BUFFER_SIZE, &bytesRead, NULL);
    if (!result || bytesRead == 0) {
        wprintf(L"Ошибка чтения данных\n");
        CloseHandle(hPipe);
        return 1;
    }

    wprintf(L"Ответ от сервера: %s\n", buffer);

    CloseHandle(hPipe);
    return 0;
}

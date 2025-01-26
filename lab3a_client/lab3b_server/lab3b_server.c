#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#define DEFAULT_SEMAPHORE_NAME L"lab3Semaphore"
#define DEFAULT_PIPE_NAME L"\\\\.\\pipe\\myPipe"
#define BUFFER_SIZE 512

HANDLE hSemaphore;

void HandleClient(HANDLE hPipe) {
    wchar_t buffer[BUFFER_SIZE];
    DWORD bytesRead;
    BOOL result;

    while (1) {
        WaitForSingleObject(hSemaphore, INFINITE);

        result = ReadFile(hPipe, buffer, BUFFER_SIZE, &bytesRead, NULL);
        if (!result || bytesRead == 0) {
            DWORD error = GetLastError();
            if (error == ERROR_BROKEN_PIPE) {
                wprintf(L"Клиент отключился\n");
            }
            else {
                wprintf(L"Ошибка чтения данных: %d\n", error);
            }
            ReleaseSemaphore(hSemaphore, 1, NULL);
            break;
        }

        wprintf(L"Получено сообщение: %Ls\n", buffer);

        for (int i = 0; i < wcslen(buffer) * 2; i++) buffer[i] = toupper(buffer[i]);

        if (!WriteFile(hPipe, buffer, bytesRead * 2, &bytesRead, NULL)) {
            wprintf(L"Ошибка отправки данных\n");
            ReleaseSemaphore(hSemaphore, 1, NULL);
            break;
        }

        ReleaseSemaphore(hSemaphore, 1, NULL);
    }

    CloseHandle(hPipe);
}

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "");
    HANDLE hPipe;
    BOOL result;

    fwide(stderr, 1);

    if (argc < 2) {
        fwprintf(stderr, L"Использование [Имя канала]");
        return EXIT_FAILURE;
    }

    const wchar_t* inputString = argv[1];
    const wchar_t* pipeName = (argc > 2) ? argv[2] : DEFAULT_PIPE_NAME;
    const wchar_t* semaphoreName = (argc > 3) ? argv[3] : DEFAULT_SEMAPHORE_NAME;

    hSemaphore = CreateSemaphoreW(NULL, 1, 1, semaphoreName);

    while (1) {
        hPipe = CreateNamedPipeW(
            pipeName,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            BUFFER_SIZE,
            BUFFER_SIZE,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            wprintf(L"Ошибка создания канала\n");
            return 1;
        }

        wprintf(L"Ожидание подключения клиента...\n");

        result = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (!result) {
            wprintf(L"Ошибка подключения клиента\n");
            CloseHandle(hPipe);
            continue;
        }

        wprintf(L"Клиент подключен\n");

        HandleClient(hPipe);
    }

    CloseHandle(hSemaphore);
    return 0;
}

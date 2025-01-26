#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

#define MAX_PROCESSES 10
//args
void convertToWideChar(const char* input, wchar_t* output, int outputSize) {
    MultiByteToWideChar(CP_UTF8, 0, input, -1, output, outputSize);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    printf("Server is running.........\n");

  
    if (argc < 4) {
        printf("Usage: %s <process count> <lower bound> <upper bound> [mutex name]\n", argv[0]);
        return 1;
    }


    int process_count = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);

    const char* defaultMutexName = "DefaultMutex";
    const char* mutex_name = defaultMutexName;

    wchar_t wideMutexName[256];

    if (argc > 4) {
        mutex_name = argv[4];

        convertToWideChar(mutex_name, wideMutexName, sizeof(wideMutexName) / sizeof(wideMutexName[0]));
    }
    else {
        wchar_t envMutexName[32];
        DWORD result = GetEnvironmentVariable(L"MUTEX_NAME", envMutexName, sizeof(envMutexName) / sizeof(envMutexName[0]));

        if (result > 0 && result < sizeof(envMutexName)) {
            wcscpy(wideMutexName, envMutexName);
        }
    }

    // Create pipe
    HANDLE hWritePipe, hReadPipe;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;  // защита по умолчанию
    sa.bInheritHandle = TRUE;        // дескрипторы наследуемые

    if (!CreatePipe(
        &hReadPipe,  // дескриптор для чтения
        &hWritePipe, // дескриптор для записи
        &sa,   // атрибуты защиты по умолчанию, дескрипторы наследуемые
        0))    // размер буфера по умолчанию
    {
        printf("Create pipe failed.\n");
        return GetLastError();
    }

    // mutex
    HANDLE  hMutex;

    hMutex = CreateMutex(NULL, FALSE, wideMutexName);
    if (hMutex == NULL)
    {
        printf("Create mutex error");
        return GetLastError();
    }

    // processes
    HANDLE childProcesses[MAX_PROCESSES];
    int processCount = 0;

    for (size_t i = 0; i < process_count; i++) {
        int rangeSize = (upper_bound - lower_bound + 1) / process_count;
        int start = lower_bound + i * rangeSize;
        int end = (i == process_count - 1) ? upper_bound : (start + rangeSize - 1);

        wchar_t command_line[128];

        wsprintf(command_line, L"D:\\5_SEM_LABS\\SP\\Lab03\\Lab-03Clang\\Lab-03a-client\\Lab-03a-client %d %d %s", start, end, wideMutexName);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.dwFlags = STARTF_USESTDHANDLES;
        si.cb = sizeof(STARTUPINFO);

        si.hStdInput = hReadPipe;
        si.hStdOutput = hWritePipe;
        si.hStdError = hWritePipe;

        if (!CreateProcess(
            NULL,
            command_line,
            NULL,
            NULL,
            TRUE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi)) {
            printf("Create process error\n");
            return GetLastError();
        }

        childProcesses[processCount++] = pi.hProcess;
        CloseHandle(pi.hThread);
    }

    // output
    for (int i = 0; i < processCount;)
    {
        char buff[1024];
        DWORD dwBytesRead;
        if (!ReadFile(
            hReadPipe,
            buff,
            sizeof(buff),
            &dwBytesRead,
            NULL))
        {
            printf("Read data error");
            return GetLastError();
        }

        WaitForSingleObject(hMutex, INFINITE);

        buff[dwBytesRead] = '\0';

        printf("%s\n\n", buff);

        i++;

        ReleaseMutex(hMutex);
    }

    for (int i = 0; i < processCount; i++) {
        CloseHandle(childProcesses[i]);
    }

    CloseHandle(hMutex);

    return 0;
}
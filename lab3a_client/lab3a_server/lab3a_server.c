#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#define BUFFER_SIZE 1024

void create_child_process(int lower_bound, int upper_bound, const wchar_t* mutex_name, HANDLE* process_handle) {
    wchar_t command[BUFFER_SIZE];
    _snwprintf(command, BUFFER_SIZE, L"D:\\Labs\\sem5\\Системное программирование\\lab3\\out\\build\\x64-release\\lab3a_client\\lab3a_client.exe %d %d %s", lower_bound, upper_bound, mutex_name);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    //printf("\nprime from %d to %d\n", lower_bound, upper_bound);

    if (!CreateProcessW(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, (LPSTARTUPINFOW)&si, &pi)) {
        fwprintf(stderr, L"Ошибка создания процесса: %d\n", GetLastError());
        return;
    }

    *process_handle = pi.hProcess;
    CloseHandle(pi.hThread);
}

int wmain(int argc, wchar_t* argv[]) {
    setlocale(LC_ALL, "");
    fwide(stderr, 1);

    if (argc < 4) {
        fwprintf(stderr, L"Использование: %Ls <количество процессов> <нижняя граница> <верхняя граница> [имя мьютекса]\n", argv[0]);
        return 1;
    }

    int num_processes = _wtoi(argv[1]);
    int lower_bound = _wtoi(argv[2]);
    int upper_bound = _wtoi(argv[3]);
    wchar_t* mutex_name = (argc > 4) ? argv[4] : (wchar_t*)L"Global\\DefaultMutex";

    HANDLE hMutex = CreateMutexW(NULL, FALSE, mutex_name);
    if (hMutex == NULL) {
        fwprintf(stderr, L"Ошибка создания мьютекса: %d\n", GetLastError());
        return 1;
    }

    int range = (upper_bound - lower_bound + 1) / num_processes;
    HANDLE process_handles[num_processes];

    for (int i = 0; i < num_processes; i++) {
        int start = lower_bound + i * range;
        int end = (i == num_processes - 1) ? upper_bound : start + range - 1;
        create_child_process(start, end, mutex_name, &process_handles[i]);
    }

    WaitForMultipleObjects(num_processes, process_handles, TRUE, INFINITE);

    for (int i = 0; i < num_processes; i++) {
        CloseHandle(process_handles[i]);
    }

    CloseHandle(hMutex);
    return 0;
}

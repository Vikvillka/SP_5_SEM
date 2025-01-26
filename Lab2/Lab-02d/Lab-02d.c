#include <stdio.h>
#include <windows.h>

void StartChildProcess(wchar_t* processName, PROCESS_INFORMATION* pi) {
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si); 
    ZeroMemory(pi, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(NULL, processName, NULL, NULL, FALSE, NULL, NULL, NULL, &si, pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return;
    }
}

int main() {
    DWORD processId = GetCurrentProcessId();

    printf("Main Process Lab02d. Process ID: %lu\n", processId);

    wchar_t processName1[] = L"D:\\5_SEM_LABS\\SP\\Lab2\\x64\\Debug\\Lab-02inf.exe";


    PROCESS_INFORMATION pi1;

    StartChildProcess(processName1, &pi1);

    DWORD pid = GetProcessId(pi1.hProcess);
    if (pid == 0) {
        printf("Failed to get PID (process is terminated). Error: %lu\n", GetLastError());
    }
    else {
        printf("Child process PID: %lu\n", pid);
    }

    printf("Press any key to terminate the child process...\n");
    getchar();


    if (!TerminateProcess(pi1.hProcess, 0)) {
        printf("TerminateProcess failed (%lu).\n", GetLastError());
    }
    else {
        printf("Child process terminated.\n");
    }

    printf("Press any key to get the PID of the terminated process...\n");
    getchar();


    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);


    pid = GetProcessId(pi1.hProcess);
    
    printf("Child process PID after CloseHandle: %lu\n", pid);
    

    printf("Press any key to exit...\n");
    getchar();
    return 0;


    printf("Main Process OS03_02 finished.\n");
    return 0;
}
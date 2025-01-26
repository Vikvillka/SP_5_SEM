#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <wtypes.h>
#include <processthreadsapi.h>
#include <WinBase.h>
#include <windows.h>

void CreateChildProcess(const wchar_t* appName, const wchar_t* cmdLine, STARTUPINFO si, PROCESS_INFORMATION pi) {

    if (!CreateProcess(appName, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        ExitProcess(1);
    }
}

int main() {

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    wchar_t path[256] = L"D:\\5_SEM_LABS\\SP\\Lab2\\x64\\Debug\\Lab-02x.exe";
    wchar_t buff[256] = L"";
    wcscpy(buff, path);
    STARTUPINFO si1 = { sizeof(si1) };
    STARTUPINFO si2 = { sizeof(si2) };
    STARTUPINFO si3 = { sizeof(si3) };
    PROCESS_INFORMATION pi1, pi2, pi3;

    ZeroMemory(&pi1, sizeof(pi1));
    ZeroMemory(&pi2, sizeof(pi2));
    ZeroMemory(&pi3, sizeof(pi3));

    CreateChildProcess(path, NULL, si1, pi1);

    CreateChildProcess(NULL, wcscat(buff, L" 100"), si2, pi2);

    LPCSTR varName = "ITER_NUM";
    LPCSTR varValue = "200";
    SetEnvironmentVariableA(varName, varValue);

    CreateChildProcess(path, NULL, si3, pi3);

    HANDLE processes[] = { pi1.hProcess, pi2.hProcess, pi3.hProcess };
    WaitForMultipleObjects(3, processes, TRUE, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);

    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    CloseHandle(pi3.hProcess);
    CloseHandle(pi3.hThread);

    return 0;
}

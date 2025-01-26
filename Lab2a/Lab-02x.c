#include <stdio.h>
#include <stdlib.h>
#include <wtypes.h>


int main(int argc, char* argv[]) {

    int n = 0;

    if (argc == 2) {
        n = atoi(argv[1]);
    }
    else {
        char buffer[256];
        DWORD bufferSize = sizeof(buffer);
        DWORD result = GetEnvironmentVariableA("ITER_NUM", buffer, bufferSize);
        if (result != NULL) {
            n = atoi(buffer);
        }
        else {
            fprintf(stderr, "Error: no arguments.\n");
            ExitProcess(1);
        }
    }

    printf("Iterations quantity: %d\n", n);

    DWORD id = GetCurrentProcessId();

    for (int i = 0; i < n; i++) {
        printf("Iteration %d, PID: %u\n", i + 1, id);
        Sleep(500);
    }

    return 0;
}

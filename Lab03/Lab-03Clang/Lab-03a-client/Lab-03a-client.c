#include <stdio.h>
#include <windows.h>
#include <locale.h>

int primes_buffer[1024];
int primes_buffer_size = 0;

void convertToWideChar(const char* input, wchar_t* output, int outputSize) {
    MultiByteToWideChar(CP_UTF8, 0, input, -1, output, outputSize);
}

BOOL isPrime(int num) {
    if (num <= 1) return FALSE;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return FALSE;
    }
    return TRUE;
}

void findPrimes(int lowerBound, int upperBound) {
    for (int i = lowerBound; i <= upperBound; ++i) {
        if (isPrime(i)) {
            primes_buffer[primes_buffer_size] = i;
            primes_buffer_size++;
        }
    }
}

void writePrimes(HANDLE hMutex) {
    WaitForSingleObject(hMutex, INFINITE);
    for (size_t i = 0; i < primes_buffer_size; i++) {
        printf("%d ", primes_buffer[i]);
    }
    ReleaseMutex(hMutex);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <lower bound> <upper bound> [mutex name]\n", argv[0]);
        return 1;
    }

    int lower_bound = atoi(argv[1]);
    int upper_bound = atoi(argv[2]);
    const char* mutex_name = (argc > 3) ? argv[3] : "DefaultMutex";

    wchar_t wideMutexName[256];
    convertToWideChar(mutex_name, wideMutexName, sizeof(wideMutexName) / sizeof(wideMutexName[0]));

    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, wideMutexName);
    if (hMutex == NULL) {
        fprintf(stderr, "Open mutex error: %ld\n", GetLastError());
        return 1;
    }

    findPrimes(lower_bound, upper_bound);

    writePrimes(hMutex);

    CloseHandle(hMutex);

    return 0;
}
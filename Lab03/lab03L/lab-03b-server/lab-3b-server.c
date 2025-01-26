#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctype.h>

#define DEFAULT_FIFO_NAME "/tmp/default_fifo"
#define DEFAULT_SEM_NAME "/default_sem"
#define BUFFER_SIZE 256

void to_uppercase(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main(int argc, char* argv[]) {
    const char* fifo_name = (argc > 1) ? argv[1] : DEFAULT_FIFO_NAME;
    const char* sem_name = DEFAULT_SEM_NAME;
    char buffer[BUFFER_SIZE];

    if (access(fifo_name, F_OK) == -1) {
        if (mkfifo(fifo_name, 0666) == -1) { //чтение+запись для user group others
            perror("mkfifo");
            return 1;
        }
    }

    sem_t* sem = sem_open(sem_name, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int fd = open(fifo_name, O_RDONLY);
        if (fd == -1) {
            perror("open");
            continue;
        }

        read(fd, buffer, BUFFER_SIZE);
        close(fd);

        printf("Received: %s\n", buffer);
        fflush(stdout);
        to_uppercase(buffer);

        fd = open(fifo_name, O_WRONLY);
        ssize_t write_result = write(fd, buffer, strlen(buffer) + 1); 
        if (write_result == -1) {  
            perror("write");
        }
        close(fd);
        printf("Sent: %s\n\n", buffer);
        fflush(stdout);
    }

    sem_close(sem);
    sem_unlink(sem_name);
    unlink(fifo_name);

    return 0;
}
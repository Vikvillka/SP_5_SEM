#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define DEFAULT_FIFO_NAME "/tmp/default_fifo"
#define DEFAULT_SEM_NAME "/default_sem"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
 
    const char *message = "hello world";
    const char *fifo_name = DEFAULT_FIFO_NAME;
    const char *sem_name = DEFAULT_SEM_NAME;

    char buffer[BUFFER_SIZE];

    sem_t *sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    sem_wait(sem);//уменьшает значение

    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        perror("open");
        sem_post(sem);//разбл
        exit(EXIT_FAILURE);
    }

    write(fd, message, strlen(message) + 1);
    close(fd);

    sleep(5);

    fd = open(fifo_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        sem_post(sem);
        exit(EXIT_FAILURE);
    }

    read(fd, buffer, BUFFER_SIZE);
    close(fd);

    printf("Response: %s\n", buffer);

    sem_post(sem);
    sem_close(sem);

    return 0;
}

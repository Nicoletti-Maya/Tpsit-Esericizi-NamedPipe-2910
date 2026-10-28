#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/mia_fifo"
#define BUF_SIZE  256

int main() {
    printf("[Reader] Attendo un writer...\n");
    fflush(stdout);

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Errore open reader");
        exit(EXIT_FAILURE);
    }

    printf("[Reader] Writer connesso. Lettura in corso...\n");

    char buffer[BUF_SIZE];
    ssize_t n = read(fd, buffer, sizeof(buffer));

    if (n > 0)
        printf("[Reader] Ricevuto: %s\n", buffer);
    else if (n == 0)
        printf("[Reader] Il writer ha chiuso senza inviare dati.\n");
    else
        perror("Errore read");

    close(fd);
    
    if (unlink(FIFO_PATH) == -1)
        perror("Errore unlink");
    else
        printf("[Reader] FIFO rimossa dal filesystem.\n");

    return 0;
}
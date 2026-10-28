#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/echo_fifo"
#define BUF_SIZE  256

int main(void) {
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] In attesa di un client...\n");
    fflush(stdout);

    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) { perror("Errore open"); exit(EXIT_FAILURE); }

    char buf[BUF_SIZE];
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        buf[strcspn(buf, "\n")] = '\0';      
        printf("[SERVER] %s\n", buf);
    } else if (n == 0) {
        printf("[SERVER] Il client ha chiuso senza inviare dati.\n");
    } else {
        perror("Errore read");
    }

    close(fd);
    if (unlink(FIFO_PATH) == -1) perror("Errore unlink");
    return 0;
}
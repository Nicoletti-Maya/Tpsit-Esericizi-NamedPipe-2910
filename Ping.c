#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PING "/tmp/fifo_ping"
#define FIFO_PONG "/tmp/fifo_pong"
#define BUF_SIZE  64

static void crea_fifo(const char *path) {
    if (mkfifo(path, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    crea_fifo(FIFO_PING);
    crea_fifo(FIFO_PONG);

    printf("[PING] Attendo pong...\n");
    fflush(stdout);
    int fd_ping = open(FIFO_PING, O_WRONLY);
    if (fd_ping == -1) { perror("open fifo_ping"); exit(EXIT_FAILURE); }

    const char *msg = "PING";
    printf("[PING] Invio: %s\n", msg);
    if (write(fd_ping, msg, strlen(msg) + 1) == -1) { perror("write"); exit(EXIT_FAILURE); }
    close(fd_ping);

    int fd_pong = open(FIFO_PONG, O_RDONLY);
    if (fd_pong == -1) { perror("open fifo_pong"); exit(EXIT_FAILURE); }

    char buf[BUF_SIZE];
    ssize_t n = read(fd_pong, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("[PING] Risposta ricevuta: %s\n", buf);
    } else if (n == 0) {
        printf("[PING] pong ha chiuso senza rispondere.\n");
    } else {
        perror("read");
    }
    close(fd_pong);

    unlink(FIFO_PING);
    unlink(FIFO_PONG);
    return 0;
}
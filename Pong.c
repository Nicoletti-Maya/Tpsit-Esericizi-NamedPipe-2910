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

    printf("[PONG] Attendo ping...\n");
    fflush(stdout);
    int fd_ping = open(FIFO_PING, O_RDONLY);
    if (fd_ping == -1) { perror("open fifo_ping"); exit(EXIT_FAILURE); }

    char buf[BUF_SIZE];
    ssize_t n = read(fd_ping, buf, sizeof(buf) - 1);
    if (n <= 0) {
        if (n == -1) perror("read");
        else fprintf(stderr, "[PONG] ping ha chiuso senza inviare dati.\n");
        exit(EXIT_FAILURE);
    }
    buf[n] = '\0';
    printf("[PONG] Ricevuto: %s\n", buf);
    close(fd_ping);

    int fd_pong = open(FIFO_PONG, O_WRONLY);
    if (fd_pong == -1) { perror("open fifo_pong"); exit(EXIT_FAILURE); }

    const char *risposta = "PONG";
    printf("[PONG] Invio: %s\n", risposta);
    if (write(fd_pong, risposta, strlen(risposta) + 1) == -1) perror("write");
    close(fd_pong);
    return 0;
}
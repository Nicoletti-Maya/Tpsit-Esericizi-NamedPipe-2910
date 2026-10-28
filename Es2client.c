#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/echo_loop_fifo"
#define BUF_SIZE  256

int main(void) {
    signal(SIGPIPE, SIG_IGN);

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("[CLIENT] Attendo il server...\n");
    fflush(stdout);
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) { perror("Errore open"); exit(EXIT_FAILURE); }

    printf("[CLIENT] Connesso. Scrivi messaggi (QUIT per uscire).\n");

    char line[BUF_SIZE];
    char msg[BUF_SIZE + 2];
    while (1) {
        printf("> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        line[strcspn(line, "\n")] = '\0';

         snprintf(msg, sizeof(msg), "%s\n", line);

        if (write(fd, msg, strlen(msg)) == -1) {
            if (errno == EPIPE)
                fprintf(stderr, "[CLIENT] Nessun server in ascolto: canale chiuso.\n");
            else
                perror("Errore write");
            break;
        }

        if (strcmp(line, "QUIT") == 0)
            break;
    }

    close(fd);
    printf("[CLIENT] Terminato.\n");
    return 0;
}
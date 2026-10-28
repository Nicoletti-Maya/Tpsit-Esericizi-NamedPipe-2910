#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_A_TO_B "/tmp/chat_a_to_b"
#define FIFO_B_TO_A "/tmp/chat_b_to_a"
#define BUF_SIZE    256

static void crea_fifo(const char *path) {
    if (mkfifo(path, 0666) == -1 && errno != EEXIST) {
        perror("Errore mkfifo");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    signal(SIGPIPE, SIG_IGN);
    crea_fifo(FIFO_A_TO_B);
    crea_fifo(FIFO_B_TO_A);

    printf("[A] Attendo B...\n");
    fflush(stdout);

    int fd_out = open(FIFO_A_TO_B, O_WRONLY);
    if (fd_out == -1) { perror("open A->B"); exit(EXIT_FAILURE); }
    int fd_in = open(FIFO_B_TO_A, O_RDONLY);
    if (fd_in == -1) { perror("open B->A"); exit(EXIT_FAILURE); }

    FILE *in = fdopen(fd_in, "r");
    if (in == NULL) { perror("fdopen"); exit(EXIT_FAILURE); }

    printf("[A] Chat avviata. Scrivi EXIT per terminare. Parli per primo.\n");

    char line[BUF_SIZE], msg[BUF_SIZE + 2], resp[BUF_SIZE];
    while (1) {
        printf("[A] > ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            strcpy(line, "EXIT");
        line[strcspn(line, "\n")] = '\0';

        snprintf(msg, sizeof(msg), "%s\n", line);
        if (write(fd_out, msg, strlen(msg)) == -1) {
            if (errno == EPIPE) fprintf(stderr, "[A] B e' sparito: canale chiuso.\n");
            else perror("Errore write");
            break;
        }
        if (strcmp(line, "EXIT") == 0) break;

        printf("[A] Attendo la risposta di B...\n");
        if (fgets(resp, sizeof(resp), in) == NULL) {
            printf("[A] B ha chiuso la connessione.\n");
            break;
        }
        resp[strcspn(resp, "\n")] = '\0';
        printf("[B] %s\n", resp);
        if (strcmp(resp, "EXIT") == 0) {
            printf("[A] B ha terminato la chat.\n");
            break;
        }
    }

    fclose(in);
    close(fd_out);
    unlink(FIFO_A_TO_B);
    unlink(FIFO_B_TO_A);
    printf("[A] Chat terminata.\n");
    return 0;
}
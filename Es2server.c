#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/echo_loop_fifo"
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

    FILE *in = fdopen(fd, "r");
    if (in == NULL) { perror("Errore fdopen"); exit(EXIT_FAILURE); }

    printf("[SERVER] Client connesso.\n");

    char buf[BUF_SIZE];
    int quit = 0;
    while (fgets(buf, sizeof(buf), in) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, "QUIT") == 0) {
            printf("[SERVER] Ricevuto QUIT. Arrivederci!\n");
            quit = 1;
            break;
        }
        printf("[SERVER] %s\n", buf);
    }

    if (!quit)
        printf("[SERVER] Il client si e' disconnesso senza QUIT. Termino.\n");

    fclose(in);
    if (unlink(FIFO_PATH) == -1) perror("Errore unlink");
    return 0;
}
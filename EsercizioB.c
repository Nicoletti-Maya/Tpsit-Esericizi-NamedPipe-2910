#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "/tmp/mia_fifo"

int main() {
    // crea fifo
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Errore mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Writer] FIFO pronta. Attende un reader...\n");
    fflush(stdout);   // diciamo che "obbliga" la stampa prima del blocco

    // Apre in sola scrittura e si blocca finche  non arriva il reader
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore open writer");
        exit(EXIT_FAILURE);
    }

    printf("[Writer] Reader connesso. Invia messaggio...\n");

    // Scrive il messaggio nella FIFO
    const char *msg = "Ciao dal processo Writer!";
    write(fd, msg, strlen(msg) + 1);

    close(fd);
    printf("[Writer] Messaggio inviato. Canale chiuso.\n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/fifo_test"

int main(void) {
    // Crea fifo, se esiste già --> stampa msg e va avanti
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Errore mkfifo");
            exit(EXIT_FAILURE);
        }
        printf("La FIFO esiste gia'\n");
    } else {
        printf("FIFO creata.\n");
    }

    // stamoa il percorso
    printf("Percorso della FIFO: %s\n", FIFO_PATH);
    printf("Sleep x 10 secondi: in un altro terminale esegui  ls -l %s\n", FIFO_PATH);
    fflush(stdout);

    sleep(10);

    // rimuove la fifo
    if (unlink(FIFO_PATH) == -1) {
        perror("Errore unlink");
        exit(EXIT_FAILURE);
    }
    printf("FIFO rimossa\n");
    return 0;
}
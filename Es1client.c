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

    char line[BUF_SIZE];
    printf("Scrivi una riga: ");
    fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Nessun input.\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) { perror("Errore open"); exit(EXIT_FAILURE); }

    if (write(fd, line, strlen(line)) == -1) perror("Errore write");

    close(fd);
    return 0;
}
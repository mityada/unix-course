#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "readlines.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: readlines-main <max_size>\n");
        return 0;
    }

    size_t max_size = atoi(argv[1]);

    RL *rl = rl_open(0, max_size);

    char *buf = malloc(max_size + 1);

    ssize_t read = 0;

    while ((read = rl_readline(rl, buf, max_size + 1)) != 0) {
        if (read == -1) {
            fprintf(stderr, "Error!\n");
            break;
        }

        if (read == -2) {
            fprintf(stderr, "The line does not fit in the buffer\n");
            continue;
        }

        if (read == -3) {
            fprintf(stderr, "Line is too long\n");
            continue;
        }

        size_t written = 0;

        while (written < read)
            written += write(1, &buf[written], read - written);
    }

    rl_close(rl);

    return 0;
}

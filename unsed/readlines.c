#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "readlines.h"

typedef struct RL {
    int fd;
    char *buf;
    size_t max_size;
    size_t chars_in_buf;
    int skip;
} RL;

RL* rl_open(int fd, size_t max_size) {
    RL *rl = malloc(sizeof(RL));
    if (!rl)
        return NULL;

    rl->fd = fd;
    rl->max_size = max_size;
    rl->buf = malloc(max_size + 1);
    if (!rl->buf) {
        free(rl);
        return NULL;
    }

    rl->chars_in_buf = 0;
    rl->skip = 0;

    return rl;
}

size_t rl_max_size(RL *rl) {
    if (!rl)
        return 0;

    return rl->max_size;
}

int rl_close(RL *rl) {
    if (!rl)
        return 0;

    if (!close(rl->fd))
        return -1;

    free(rl->buf);
    free(rl);

    return 0;
}

int rl_readline(RL *rl, char *buf, size_t buf_size) {
    if (!rl)
        return -1;

    ssize_t bytes_read = rl->chars_in_buf;

    if (!bytes_read)
        bytes_read = read(rl->fd, rl->buf, rl->max_size + 1);

    if (!bytes_read)
        return bytes_read;

    size_t length = 0;

    while (1) {
        while (rl->buf[length] != '\n' && length < bytes_read)
            length++;

        if (rl->buf[length] == '\n') {
            length++;

            int result;

            if (rl->skip) {
                rl->skip = 0;
                result = -3;
            } else {
                if (length <= buf_size) {
                    memcpy(buf, rl->buf, length);
                    result = length;
                } else {
                    result = -2;
                }
            }

            memmove(rl->buf, &rl->buf[length], bytes_read - length);
            rl->chars_in_buf = bytes_read - length;
            return result;
        } else {
            if (length == rl->max_size + 1) {
                rl->skip = 1;
                rl->chars_in_buf = 0;
                return -3;
            } else {
                bytes_read += read(rl->fd, &rl->buf[length], rl->max_size + 1 - length);
            }
        }
    }
}

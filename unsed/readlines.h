#ifndef _READLINES_H
#define _READLINES_H

typedef struct RL RL;

RL* rl_open(int fd, size_t max_size);

size_t rl_max_size(RL *rl);

int rl_close(RL *rl);

int rl_readline(RL *rl, char *buf, size_t buf_size);

#endif

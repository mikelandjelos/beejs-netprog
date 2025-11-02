/*
** server.c -- a stream socket server demo
** Test with `telnet <hostname> 3490`
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"

#define BACKLOG 10 // how many pending connections will client queue hold

#define MAX_MSG_LEN 1025

#define SUCCESS 0
#define ERR_INIT -1
#define ERR_CHLD -2
#define ERR_COMM -3

void sigchld_handler(int s)
{
    (void)s;                 // no unused variable warning
    int saved_errno = errno; // because waitpid may overwrite it
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved_errno;
}

// get sockaddr - void pointer return because it can be ipv6 or ipv4
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main()
{
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use this host IP

    int status;
    if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)), exit(ERR_INIT);

    int sockfd, yes = 1;
    struct addrinfo *p;

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            perror("setsockopt"), exit(ERR_INIT);

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break; // INITIALIZED
    }

    freeaddrinfo(servinfo); // using structure finished

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(ERR_INIT);
    }

    if (listen(sockfd, BACKLOG) == -1)
        perror("server: listen"), exit(ERR_INIT);

    struct sigaction sa;
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        perror("sigaction"), exit(ERR_CHLD);

    printf("server: Waiting for connections...\n");

    int new_fd;
    struct sockaddr_storage client_addr;
    socklen_t sin_size = sizeof(struct sockaddr_storage);
    char net_addr_repr[NI_MAXHOST];
    char transport_addr_repr[NI_MAXSERV];

    while (1) {
        new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (new_fd == -1) {
            perror("server: accept");
            continue;
        }

        getnameinfo((struct sockaddr *)&client_addr, sin_size, net_addr_repr, NI_MAXHOST, transport_addr_repr,
                    NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        printf("server: accepting connection from %s:%s\n", net_addr_repr, transport_addr_repr);

        if (!fork()) {     // child process - worker
            close(sockfd); // don't need the listener
            int recv_len, count = 1;
            char msg[MAX_MSG_LEN];
            pid_t pid = getpid();
            while (1) {
                memset(msg, 0, MAX_MSG_LEN);
                recv_len = recv(new_fd, msg, MAX_MSG_LEN, 0);
                if (recv_len == 0) // closed connection
                    break;
                if (recv_len == -1)
                    perror("worker: recv"), exit(ERR_COMM);
                if (send(new_fd, msg, strnlen(msg, MAX_MSG_LEN), 0) == -1)
                    perror("worker: send"), exit(ERR_COMM);
                printf("[%d] Echoed message number %d\n", pid, count++);
                fflush(stdout);
            }
            close(new_fd);
            printf("[%d] Process finished execution\n", pid);
            fflush(stdout);
            exit(SUCCESS);
        }

        close(new_fd); // server doesn't need this
    }

    return 0;
}
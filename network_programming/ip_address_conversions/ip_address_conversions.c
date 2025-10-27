#include <stdio.h>
#include <sys/socket.h> // for AF_INET, AF_INET6
#include <netinet/in.h> // for struct sockaddr_in, sockaddr_in6
#include <arpa/inet.h>  // for inet_pton()

int main()
{
    // IPv4:
    struct sockaddr_in sa; // IPv4
    if (inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)) < 0) {
        perror("inet_pton IPv4");
    }
    char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string

    if (!inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN)) {
        perror("inet_ntop IPv4");
    }
    printf("The IPv4 address is: %s\n", ip4);

    // IPv6:
    struct sockaddr_in6 sa6; // IPv6
    if (inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)) < 0) {
        perror("inet_pton IPv6");
    }
    char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string

    if (!inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN)) {
        perror("inet_ntop IPv6");
    }
    printf("The address is: %s\n", ip6);

    return 0;
}
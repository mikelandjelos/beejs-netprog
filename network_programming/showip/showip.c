/*
 ** showip.c
 **
 ** Show IP addresses for a host given on the command line
 **
 ** Example usage: `./dist/showip "google.com" ipver4 | xargs ping -c 4`
 ** Output:
 ** ```txt
 ** PING 142.250.180.206 (142.250.180.206) 56(84) bytes of data.
 ** 64 bytes from 142.250.180.206: icmp_seq=1 ttl=115 time=12.6 ms
 ** 64 bytes from 142.250.180.206: icmp_seq=2 ttl=115 time=12.3 ms
 ** 64 bytes from 142.250.180.206: icmp_seq=3 ttl=115 time=12.4 ms
 ** 64 bytes from 142.250.180.206: icmp_seq=4 ttl=115 time=12.4 ms
 **
 ** --- 142.250.180.206 ping statistics ---
 ** 4 packets transmitted, 4 received, 0% packet loss, time 3004ms
 ** rtt min/avg/max/mdev = 12.326/12.424/12.575/0.092 ms
 ** ```
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define ERROR_BAD_ARGUMENTS -1

typedef enum IpVersion {
    Version4 = AF_INET,
    Version6 = AF_INET6,
    Unspecified = AF_UNSPEC,
} IpVersion;

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;

    if (argc < 2 || argc > 3)
        fprintf(stderr, "usage: showip <hostname> [IPv4 | IPv6]\n"),
            fflush(stderr), exit(ERROR_BAD_ARGUMENTS);

    IpVersion ipver = Unspecified;
    if (argc == 3) {
        int len = 0;
        char *s = argv[2];
        while (*s)
            ++len, ++s;
        --len, --s;
        if (len > 5)
            fprintf(stderr, "version parameter needs to be max length 5\n"),
                fflush(stderr), exit(ERROR_BAD_ARGUMENTS);
        if (*s == '4' || *s == '6')
            ipver = *s == '4' ? Version4 : Version6;
        else
            fprintf(stderr, "ip version must be 4 or 6, but provided %c\n", *s),
                fflush(stderr), exit(ERROR_BAD_ARGUMENTS);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = ipver; // IP version agnostic
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(argv[1], "443", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(-2);
    }

    void *addr;
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    char ipstr[INET6_ADDRSTRLEN];
    for (p = res; p != NULL; p = p->ai_next) {
        // get the pointer to the address
        // different fields for different types
        if (p->ai_family == AF_INET) {
            ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s\n", ipstr);
    }

    freeaddrinfo(res);

    return 0;
}
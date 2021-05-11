/*
 * Copyright (C) 2021 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./zadanie_04_ipforward.c -o ./zadanie_04_ipforward
 * Usage:        ./zadanie_04_ipforward
 * NOTE:         This program requires root privileges.
 *
 * Bug reports:  https://gitlab.cs.put.poznan.pl/mkalewski/ps-2021/issues
 *
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IPPROTO_CUSTOM 222

int main(int argc, char **argv) {
    int sfd, rc, next_sock;
    char buf[65536], saddr[16], daddr[16];
    char *data;
    socklen_t sl;
    struct sockaddr_in addr;
    struct sockaddr_in next_addr;
    struct iphdr *ip;

    sfd = socket(PF_INET, SOCK_RAW, IPPROTO_CUSTOM);

    memset(&addr, 0, sizeof(addr));
    sl = sizeof(addr);


    while (1) {
        rc = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &sl);
        ip = (struct iphdr *)&buf;
        if (ip->protocol == IPPROTO_CUSTOM) {
            inet_ntop(AF_INET, &ip->saddr, (char *)&saddr, 16);
            inet_ntop(AF_INET, &ip->daddr, (char *)&daddr, 16);
            data = (char *)ip + (ip->ihl * 4);
            printf("[%dB] %s -> %s | %s\n", rc - (ip->ihl * 4), saddr, daddr, data);
            next_sock = socket(PF_INET, SOCK_RAW, IPPROTO_CUSTOM);
            memset(&next_addr, 0, sizeof(next_addr));
            next_addr.sin_family = AF_INET;
            next_addr.sin_port = 0;
            next_addr.sin_addr.s_addr = inet_addr(argv[0]);
            sendto(next_sock, buf, strlen(buf), 0, (struct sockaddr *)&next_addr,
                   sizeof(next_addr));
            printf("[%dB] %s -> %s | %s\n", rc - (ip->ihl * 4), daddr, argv[0], data);
            close(next_sock);
        }
    }
    close(sfd);
    return EXIT_SUCCESS;
}
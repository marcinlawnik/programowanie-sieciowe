/*
 * Copyright (C) 2021 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./zadanie_04_setsockopt.c -o ./zadanie_04_setsockopt
 * Usage:        ./zadanie_04_setsockopt
 *
 * Bug reports:  https://gitlab.cs.put.poznan.pl/mkalewski/ps-2021/issues
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int getbuffsize(int sfd, int buffname) {
    int s;
    socklen_t slt = (socklen_t)sizeof(s);
    getsockopt(sfd, SOL_SOCKET, buffname, (void*)&s, &slt);
    return s;
}

void buffsizes(int sfd, int *srb, int *ssb) {
    *srb = getbuffsize(sfd, SO_RCVBUF);
    *ssb = getbuffsize(sfd, SO_SNDBUF);
}

int main(int argc, char **argv) {
    int sfd, srb, ssb;
    //ZMIANA
    int rcvbufsize, sendbufsize;
    rcvbufsize = atoi(argv[1]);
    sendbufsize = atoi(argv[2]);
    printf("%6d %6d\n", rcvbufsize, sendbufsize);
    sfd = socket(PF_INET, SOCK_STREAM, 0);
    setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &rcvbufsize, sizeof(rcvbufsize));
    setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &sendbufsize, sizeof(sendbufsize));
    //END ZMIANA
    buffsizes(sfd, &srb, &ssb);
    printf("TCP:  RCVBUF = %6d [B]  SNDBUF = %6d [B]\n", srb, ssb);
    close(sfd);
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    buffsizes(sfd, &srb, &ssb);
    printf("UDP:  RCVBUF = %6d [B]  SNDBUF = %6d [B]\n", srb, ssb);
    close(sfd);
    return EXIT_SUCCESS;
}
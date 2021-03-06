/*
 * Copyright (C) 2021 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./tcp-server.c -o ./tcp-server
 * Usage:        ./tcp-server
 *
 * Bug reports:  https://gitlab.cs.put.poznan.pl/mkalewski/ps-2021/issues
 *
 */


#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
    socklen_t sl;
    int sfd, cfd, on = 1;
    struct sockaddr_in saddr, caddr;

    int xfd, rc;
    char buf[128];
    struct sockaddr_in xaddr;
    struct hostent* addrent;

    addrent = gethostbyname(argv[1]);
    xfd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&xaddr, 0, sizeof(xaddr));
    xaddr.sin_family = AF_INET;
    xaddr.sin_port = htons(atoi(argv[2]));
    memcpy(&xaddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
    connect(xfd, (struct sockaddr*) &xaddr, sizeof(xaddr));

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd = socket(PF_INET, SOCK_STREAM, 0);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));
    bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
    listen(sfd, 5);
    while(1) {
        memset(&caddr, 0, sizeof(caddr));
        sl = sizeof(caddr);
        cfd = accept(sfd, (struct sockaddr*) &caddr, &sl);
        rc = read(cfd, buf, 128);
        //sleep(1200);
        close(cfd);

        write(xfd, buf, strlen(buf));
        memset(buf, 0, sizeof bus);
    }

    close(xfd);
    close(sfd);
    return EXIT_SUCCESS;
}



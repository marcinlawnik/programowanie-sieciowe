#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_PORT     1111

int main(int argc, char** argv) {
    int sfd, rc;
    socklen_t sl;
    char buf[128];
    struct sockaddr_in saddr, caddr, naddr;
    struct hostent* addrent;

    addrent = gethostbyname(argv[1]);

    memset(&naddr, 0, sizeof(naddr));
    naddr.sin_family = AF_INET;
    memcpy(&naddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
    naddr.sin_port = htons(atoi(argv[2]));

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(LISTEN_PORT);
    sfd = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
    while(1) {
        memset(&caddr, 0, sizeof(caddr));
        memset(&buf, 0, sizeof(buf));
        sl = sizeof(caddr);
        rc = recvfrom(sfd, buf, 128, 0, (struct sockaddr*) &caddr, &sl);
        sendto(sfd, buf, rc, 0, (struct sockaddr*) &naddr, sl);
    }
    close(sfd);
    return EXIT_SUCCESS;
}



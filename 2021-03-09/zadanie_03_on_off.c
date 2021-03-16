#include <arpa/inet.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int sfd;
    struct ifreq ifr;

    if (argc != 3) {
        printf("Invalid argument count. Use:\n%s <interface> on to turn on the interface.\n%s <interface> off to turn down the interface.\n", argv[0], argv[0]);
        exit(1);
    }

    sfd = socket(PF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ);

    ioctl(sfd, SIOCGIFFLAGS, &ifr);

    if (strcmp(argv[2], "on") == 0) {
        ifr.ifr_flags |= IFF_UP;
        ioctl(sfd, SIOCSIFFLAGS, &ifr);
        printf("%s is up.\n", argv[1]);
    }

    else if (strcmp(argv[2], "off") == 0) {
        ifr.ifr_flags &= ~IFF_UP;
        ioctl(sfd, SIOCSIFFLAGS, &ifr);
        printf("%s is down.\n", argv[1]);
    }

    else {
        printf("Invalid arguments. Use:\n%s <interface> on to turn on the interface.\n%s <interface> off to turn down the interface.\n", argv[0], argv[0]);
        exit(1);
    }

    return 0;
}
/*
 *
 * Compilation:  gcc -Wall ./zadanie_05_pcapcounter.c -o ./zadanie_05_pcapcounter -lpcap
 * Usage:        ./pcaplistener INTERFACE
 * NOTE:         This program requires root privileges.
 *
 * Description:
 * this program listens for all frames on given interface and counts the number
 * of frames per protocol
 */

#include <arpa/inet.h>
#include <ctype.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
//https://github.com/torvalds/linux/blob/master/include/uapi/linux/in.h
#include <linux/in.h>
#include <pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IP_ETHERTYPE 8
#define ARP_ETHERTYPE 1544

char *errbuf;
pcap_t *handle;

int ipCounter = 0;
int arpCounter = 0;
int tcpCounter = 0;
int udpCounter = 0;
int otherCounter = 0;

const static char *etype(unsigned int ethertype) {
    static char buf[16];
    switch (ntohs(ethertype) & 0xFFFFU) {
        case 0x0001:
            return "802_3";
        case 0x0002:
            return "AX25";
        case 0x0003:
            return "ALL";
        case 0x0060:
            return "LOOP";
        case 0x0800:
            return "IP";
        case 0x0806:
            return "ARP";
        case 0x8100:
            return "8021Q";
        case 0x88A8:
            return "8021AD";
        default:
            snprintf(buf, sizeof(buf), "0x%04x", ntohs(ethertype) & 0xFFFFU);
            return (const char *)buf;
    }
}

void cleanup() {
    printf("\n ipCounter: %d\n arpCounter: %d\n tcpCounter: %d\n udpCounter: %d\n otherCounter: %d\n", ipCounter, arpCounter, tcpCounter, udpCounter, otherCounter);
    pcap_close(handle);
    free(errbuf);
}

void stop(int signo) { exit(EXIT_SUCCESS); }

void trap(u_char *user, const struct pcap_pkthdr *h, const u_char *frameBytes) {
    struct ethhdr *eth = (struct ethhdr *)frameBytes;
    printf("~~~~~ [%d] -> [%s]\n", eth->h_proto, etype(eth->h_proto));

    if (eth->h_proto == IP_ETHERTYPE) {
        ipCounter++;
        struct iphdr *iphdr = (struct iphdr *)frameBytes + sizeof(struct ethhdr);
        printf("protocol: %d\n", iphdr->protocol);

        if (iphdr->protocol == IPPROTO_TCP) {
            tcpCounter++;
        } else if (iphdr->protocol == IPPROTO_UDP) {
            udpCounter++;
        }

    } else if (eth->h_proto == ARP_ETHERTYPE) {
        arpCounter++;
    } else {
        otherCounter++;
    }

    printf("[%dB of %dB]\n", h->caplen, h->len);
}

int main(int argc, char **argv) {
    atexit(cleanup);
    signal(SIGINT, stop);
    printf("Starting\n");
    errbuf = malloc(PCAP_ERRBUF_SIZE);
    handle = pcap_create(argv[1], errbuf);
    pcap_set_promisc(handle, 1);
    pcap_set_snaplen(handle, 65535);


    // This below forces libpcap to act immadiately upon packet arrival
    // Credit: http://codeblurbs.blogspot.com/2016/07/a-simple-libpcap-example-for-live.html
    // Deliver packets as soon as they arrive. See the pcap man page for more info.
    if( pcap_set_timeout( handle, 1 ) != 0 )
    {
        printf( "Unable to configure timeout.\n" );
        exit(EXIT_FAILURE);
    }

    // When immediate mode is enabled, reads return immediately upon packet reception.
    // Otherwise, a read will block until either the kernel buffer becomes full or a timeout occurs.
    if( pcap_set_immediate_mode( handle, 1 ) != 0 )
    {
        printf( "Unable to configure immediate mode.\n" );
        exit(EXIT_FAILURE);
    }

    pcap_activate(handle);
    pcap_loop(handle, -1, trap, NULL);
}
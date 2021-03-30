/*
 * Compilation:  gcc -Wall ./zadanie_04_arping.c -o ./zadanie_04_arping -lnet -lpcap
 * Usage:        ./zadanie_04_arping IFNAME HOST
 * NOTE:         This program requires root privileges.
 * NOTE:         Work in progress
 */

#include <arpa/inet.h>
#include <ctype.h>
#include <libnet.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char** argv) {
    libnet_t *ln;
    u_int32_t target_ip_addr, src_ip_addr;
    u_int8_t bcast_hw_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
            zero_hw_addr[6]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    struct libnet_ether_addr* src_hw_addr;
    char errbuf[LIBNET_ERRBUF_SIZE];

    ln = libnet_init(LIBNET_LINK, argv[1], errbuf);
    src_ip_addr = libnet_get_ipaddr4(ln);
    src_hw_addr = libnet_get_hwaddr(ln);
    target_ip_addr = libnet_name2addr4(ln, argv[2], LIBNET_RESOLVE);
    libnet_autobuild_arp(
            ARPOP_REQUEST,                   /* operation type       */
            src_hw_addr->ether_addr_octet,   /* sender hardware addr */
            (u_int8_t*) &src_ip_addr,        /* sender protocol addr */
            zero_hw_addr,                    /* target hardware addr */
            (u_int8_t*) &target_ip_addr,     /* target protocol addr */
            ln);                             /* libnet context       */
    libnet_autobuild_ethernet(
            bcast_hw_addr,                   /* ethernet destination */
            ETHERTYPE_ARP,                   /* ethertype            */
            ln);                             /* libnet context       */
    libnet_write(ln);
    libnet_destroy(ln);
    return EXIT_SUCCESS;
}

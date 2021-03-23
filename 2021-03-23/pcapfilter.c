/*
 * Copyright (C) 2021 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./pcapfilter.c -o ./pcapfilter -lpcap
 * Usage:        ./pcapfilter INTERFACE EXPRESSION
 * NOTE:         This program requires root privileges.
 *
 * Bug reports:  https://gitlab.cs.put.poznan.pl/mkalewski/ps-2021/issues
 *
 */

#include <pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

char* errbuf;
pcap_t* handle;

void cleanup() {
    pcap_close(handle);
    free(errbuf);
}

void stop(int signo) {
    exit(EXIT_SUCCESS);
}

void trap(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    printf("[%dB of %dB]\n", h->caplen, h->len);
}

int main(int argc, char** argv) {
    bpf_u_int32 netp, maskp;
    struct bpf_program fp;

    atexit(cleanup);
    signal(SIGINT, stop);
    errbuf = malloc(PCAP_ERRBUF_SIZE);
    handle = pcap_create(argv[1], errbuf);
    pcap_set_promisc(handle, 1);
    pcap_set_snaplen(handle, 65535);
    pcap_activate(handle);
    pcap_lookupnet(argv[1], &netp, &maskp, errbuf);
    pcap_compile(handle, &fp, argv[2], 0, maskp);
    if (pcap_setfilter(handle, &fp) < 0) {
        pcap_perror(handle, "pcap_setfilter()");
        exit(EXIT_FAILURE);
    }
    pcap_loop(handle, -1, trap, NULL);
}

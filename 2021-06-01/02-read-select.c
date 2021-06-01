/*
 * Copyright (C) 2021 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./02-read-select.c -o ./02-read-select
 * Usage:        ./02-read-select SERVER PORT
 *
 * Bug reports:  https://gitlab.cs.put.poznan.pl/mkalewski/ps-2021/issues
 *
 */

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int readable(int fd, int tout) {
  fd_set rset;
  struct timeval tv;
  FD_ZERO(&rset);
  FD_SET(fd, &rset);
  tv.tv_sec = tout;
  tv.tv_usec = 0;
  return select(fd + 1, &rset, NULL, NULL, &tv);
}

int main(int argc, char** argv) {
  int sfd, rc = 0;
  char buf[128];
  struct sockaddr_in saddr;
  struct hostent* addrent;

  addrent = gethostbyname(argv[1]);
  sfd = socket(PF_INET, SOCK_STREAM, 0);
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(atoi(argv[2]));
  memcpy(&saddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
  connect(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  if (readable(sfd, 1) > 0)
    rc = read(sfd, buf, sizeof(buf));
  else
    printf("[select] timeout\n");
  write(1, buf, rc);
  close(sfd);
  return EXIT_SUCCESS;
}

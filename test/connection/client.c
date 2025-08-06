/*--------------UNSTACK TCP---------------*/
#include "../../lib/include/inet.h"
#include "../../lib/include/netdev.h"
#include "../../lib/include/socket.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s client_addr server_addr port", argv[0]);
    exit(0);
  }

  pthread_t t;
  if (pthread_create(&t, NULL, (void *)net_device_start_loop,
                     (void *)argv[1]) != 0) {
    printf("Error in creating thread\n");
    exit(0);
  }

  struct sockaddr serv_addr;

  int port = atoi(argv[3]);

  struct socket *sock = _socket(AF_INET, SOCK_STREAM, 0);
  if (inet_pton(AF_INET, argv[1], &sock->sk->addr.src_addr) != 1) {
    printf("Error in inet_pton");
    exit(0);
  }
  sock->sk->addr.src_port = htons(24);

  if (inet_pton(AF_INET, argv[2], &serv_addr.dst_addr) != 1) {
    printf("Error in inet_pton");
    exit(0);
  }
  serv_addr.dst_port = htons(port);

  if (_connect(sock, &serv_addr) < 0) {
    perror("Could not connect\n");
    exit(1);
  }
  printf("Connected\n");
  return 0;
}

/*----------------LINUX TCP---------------*/

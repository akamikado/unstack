/*-----------------------UNSTACK TCP-----------------------*/

/*-----------------------LINUX TCP-------------------------*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s server_addr server_port", argv[0]);
    exit(0);
  }
  const int port = atoi(argv[2]);
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_sockaddr;
  socklen_t server_socklen = sizeof(server_sockaddr);
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
  server_sockaddr.sin_port = htons(port);
  if (bind(server_fd, (const struct sockaddr *)&server_sockaddr,
           server_socklen) < 0) {
    perror("Bind failed\n");
    exit(0);
  }
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed\n");
    exit(0);
  }
  struct sockaddr_in client_sockaddr;
  socklen_t client_socklen = sizeof(client_sockaddr);

  int client_fd =
      accept(server_fd, (struct sockaddr *)&client_sockaddr, &client_socklen);
  if (client_fd < 0) {
    perror("Accept failed\n");
    exit(0);
  }
  printf("Connected");
  return 0;
}

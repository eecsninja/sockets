#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const unsigned long kServerAddress = inet_addr("127.0.0.1");
const int kServerPort = 9734;

int main() {
  int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = kServerAddress;
  server_address.sin_port = kServerPort;
  bind(server_sockfd, (struct sockaddr *)&server_address,
       sizeof(server_address));

  listen(server_sockfd, 5);
  while (true) {
    printf("Server waiting.\n");

    sockaddr_in client_address;
    socklen_t addr_len;
    int client_sockfd = accept(server_sockfd,
                               reinterpret_cast<sockaddr*>(&client_address),
                               &addr_len);
    char buf[128];
    while (true) {
      memset(buf, 0, sizeof(buf));
      size_t read_size = read(client_sockfd, buf, sizeof(buf));
      for (char* c = buf; *c; ++c) {
        *c = toupper(*c);
      }
      write(client_sockfd, buf, strlen(buf) + 1);
    }
    close(client_sockfd);
  }

  return 0;
}
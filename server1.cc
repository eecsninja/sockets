#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const char kServerSocketName[] = "Simon's server";

int main() {
  unlink(kServerSocketName);
  int server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  sockaddr_un server_address;
  server_address.sun_family = AF_UNIX;
  snprintf(server_address.sun_path, sizeof(server_address.sun_path),
           kServerSocketName);
  bind(server_sockfd, (struct sockaddr *)&server_address,
       sizeof(server_address));

  listen(server_sockfd, 5);
  while (true) {
    printf("Server waiting.\n");

    sockaddr_un client_address;
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
      write(client_sockfd, buf, strlen(buf));
    }
    close(client_sockfd);
  }

  return 0;
}
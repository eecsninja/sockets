#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  const char* host = nullptr;
  if (argc <= 1)
    host = "localhost";
  else
    host = argv[1];

  hostent* hostinfo = gethostbyname(host);
  if (!hostinfo) {
    fprintf(stderr, "Error connecting to host %s.\n", host);
    return 1;
  }

  servent* servinfo = getservbyname("daytime", "tcp");
  if (!servinfo) {
    fprintf(stderr, "Host %s has no daytime service.\n", host);
    return 1;
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr = *reinterpret_cast<in_addr*>(*hostinfo->h_addr_list);
  address.sin_port = servinfo->s_port;

  printf("Connecting to %08x:%d.\n", address.sin_addr.s_addr, address.sin_port);

  int result =
      connect(sockfd, reinterpret_cast<const sockaddr*>(&address),
              sizeof(address));
  if (result == -1) {
    fprintf(stderr, "Error connecting client to %08x:%d.\n",
            address.sin_addr.s_addr, address.sin_port);
    return 1;
  }

  char buf[128];
  ssize_t size_read = read(sockfd, buf, sizeof(buf) - 1);
  buf[size_read] = '\0';
  printf("Size read: %zd\n", size_read);
  printf("Data read: %s\n", buf);

  close(sockfd);

  return 0;
}
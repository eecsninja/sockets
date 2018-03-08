#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const unsigned long kServerAddress = inet_addr("127.0.0.1");
const int kServerPort = 9734;

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    printf("Please provide input text.\n");
    return 0;
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = kServerAddress;
  address.sin_port = kServerPort;

  int result =
      connect(sockfd, reinterpret_cast<const sockaddr*>(&address),
              sizeof(address));
  if (result == -1) {
    fprintf(stderr, "Error connecting client to %08x:%.\n", kServerAddress,
            kServerPort);
    return 1;
  }

  const char* input = argv[1];
  printf("Sending input text: %s\n", input);
  write(sockfd, input, strlen(input) + 1);

  printf("Output from server: ");
  char buf[128];
  ssize_t size_read;
  while ((size_read = read(sockfd, buf, sizeof(buf))) > 0) {
    printf("Size read: %zd\n", size_read);
    printf("%s\n", buf);
  }
  printf("Size read: %zd\n", size_read);

  close(sockfd);

  return 0;
}
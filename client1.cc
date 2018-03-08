#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const char kServerSocketName[] = "Simon's server";

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    printf("Please provide input text.\n");
    return 0;
  }

  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  sockaddr_un address;
  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, sizeof(address.sun_path), kServerSocketName);

  int result =
      connect(sockfd, reinterpret_cast<const sockaddr*>(&address),
              sizeof(address));
  if (result == -1) {
    fprintf(stderr, "Error connecting client to %s.\n", address.sun_path);
    return 1;
  }

  const char* input = argv[1];
  printf("Sending input text: %s\n", input);
  write(sockfd, input, strlen(input));

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
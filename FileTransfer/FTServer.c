#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

// ./FTServer [IP] [PORT] [OUTFILE]

void writeFile(int sockfd, char *filename){
  int n;
  FILE *fp;
  char buffer[1024];
  fp = fopen(filename, "w");
  while(1){
    n = recv(sockfd, buffer, 1024, 0);
    if(n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
  }
  return;
}

int main(int argc, char *argv[]){
  const char* ip = argv[1];
  const int port = atoi(argv[2]);
  struct sockaddr_in cliAddr, servAddr;
  char buffer[1024];

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("[-] Unable to create socket, exiting..\n");
    exit(1);
  }
  printf("[*] Socket initialized\n");
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_port = htons(port);
  inet_aton(ip, &cliAddr.sin_addr);
  int e = bind(sockfd, (struct sockaddr*)&cliAddr, sizeof(cliAddr));
  if(e < 0){
    printf("[-] There was an issue while binding the socket, exiting..\n");
    exit(1);
  }
  printf("[*] Socket bound successfully\n");
  if(listen(sockfd, 10)  != 0){
    printf("[-] There was an issue while listening on %s:%d\n", ip, port);
  }
  printf("Listening on %s:%d\n",ip, port);
  socklen_t addrSize = sizeof(servAddr);
  int servSocket = accept(sockfd, (struct sockaddr*)&servAddr, &addrSize);
  writeFile(servSocket, argv[3]);
  printf("[+] Data written in the file successfully\n");
  return 0;

}

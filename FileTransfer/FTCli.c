#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

// ./FTCli [IP] [PORT] [FILE]

void sendFile(FILE* fp, int sockfd){
  int n; char data[1024] = {0};
  while(fgets(data, 1024, fp) != NULL){
    if(send(sockfd, data, sizeof(data), 0) == -1){
      printf("[-] There was an error while sending file, exiting..");
      exit(1);
    }
    bzero(data, 1024);
  }
}

int FileExists(const char *fname){
  if(access(fname, F_OK) == 0){
    return 0;
  } else{
    return 1;
  }
}

int main(int argc, char *argv[]){
  // Defining constants and structures
  const char* ip = argv[1];
  const int port = atoi(argv[2]);
  FILE *fp;
  struct sockaddr_in servAddr;

  // Check if file exists
  if(FileExists(argv[3]) == 1){
    printf("[-] Unable to access %s, exiting..\n", argv[3]);
    exit(1);
  }
  char *filename = argv[3]; // Store file name if it exists

  // Creating socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd<0){
    printf("[-] Unable to create socket, exiting\n");
    exit(sockfd);
  }
  printf("[*] Socket initialized\n");

  // Setting socket fields
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(port);
  inet_aton(ip, &servAddr.sin_ad);
  int e = connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr));
  if(e == -1){
    printf("[-] Error while connecting, exitting..\n");
    exit(1);
  }
  printf("[+] Connected to server\n");
  fp = fopen(filename, "r");
  if(fp == NULL){
    printf("[-] There was an issue reading %s, exiting..\n",filename);
    exit(1);
  }
  sendFile(fp, sockfd);
  printf("[+] File sent successfully, closing socket\n");
  close(sockfd);
  return 0;

}

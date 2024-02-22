#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include "util.h"
#include<stdio.h>
#include<unistd.h>

int main()
{
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  errif(sockfd==-1,"client:socker create error");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8888);
  
  int connectRes = connect(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr));
  errif(connectRes==-1,"client:socket connect error");

  while (true)
  {
      char buf[1024];
      bzero(&buf, sizeof(buf));
      scanf("%s",buf);
      size_t write_bytes = write(sockfd,buf, sizeof(buf));
      if(write_bytes==-1)
      {
          printf("socket already disconnect,can't write any more!");
          break;
      }
      bzero(&buf, sizeof(buf));
      ssize_t read_bytes = read(sockfd,buf, sizeof(buf));
      if(read_bytes>0)
      {
          printf("message from server: %s\n",buf);
      }else if(read_bytes==0)
      {
          printf("server socket disconnect\n");
          break;
      } else if (read_bytes ==-1)
      {
          close(sockfd);
          errif(true,"client:socket read error");
      }
  }

  close(sockfd);
  return 0;
}

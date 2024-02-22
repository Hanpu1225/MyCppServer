#include <stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include "util.h"
#include<unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd==-1,"server:socker create error");//错误提示

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    int bindRes = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    errif(bindRes==-1,"server:socket bind error");

    int listenRes = listen(sockfd, SOMAXCONN);
    errif(listenRes==-1,"server:socket listen error");

    //对于服务端来说，也要保存客户端的socket地址信息
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd==-1,"server:socket accept error");
    //打印连接到服务器的socket、ip地址、端口号
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while (true)
    {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        ssize_t  read_bytes = read(clnt_sockfd,buf,sizeof(buf));
        if(read_bytes>0)
        {
            printf("message from client fd %d: %s\n",clnt_sockfd,buf);
            write(clnt_sockfd,buf,sizeof(buf));
        }else if(read_bytes == 0)
        {
            printf("clint fd %d disconnect\n",clnt_sockfd);
            break;
        }else if(read_bytes==-1)
        {
            close(clnt_sockfd);
            errif(true,"client:socket read error");
        }
    }
    close(sockfd);

    return 0;
}

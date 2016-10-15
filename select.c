#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<assert.h>
#include<unistd.h>

#define BUF_SIZE 200

int fd_A[5];
int conn_count;

void showclient()
{
    int i;
    printf("client num:%d\n",conn_count);
    for(;i<5;i++){
        printf("[%d]:%d  ",i,fd_A[i]);
        printf("\n\n");
    }
}

int main(int argc,char* argv[])
{
    if(argc <= 2){
        printf("too low num!!!");
        exit(0);
    }

    int port = atoi(argv[2]);
    const char* ip = argv[1];

    int sockfd,newfd;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;

    char buff[BUF_SIZE];
    int i=0;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd != -1);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    socklen_t serverlen = sizeof(serveraddr);
    int res = bind(sockfd,(struct sockaddr*)&serveraddr,serverlen);
    assert(res != -1);
    res = listen(sockfd,5);
    assert(res != -1);

    fd_set fdsr;
    struct timeval tv;
    int maxsock = sockfd;
    socklen_t clientlen = sizeof(clientaddr);

    while(1){
        FD_ZERO(&fdsr);
        FD_SET(sockfd,&fdsr);

        tv.tv_sec= 30;
        tv.tv_usec = 0;

        for(i = 0; i < 5;i++){
            if(fd_A[i] != 0){
                FD_SET(fd_A[i],&fdsr);
            }
        }

        res = select(maxsock+1,&fdsr,NULL,NULL,&tv);
        assert(res != -1);

        for(i=0;i<conn_count;i++){
            if(FD_ISSET(fd_A[i],&fdsr)){
                res = recv(fd_A[i],buff,sizeof(buff),0);
                if(res < 0){
                    printf("client[%d] close\n",i);
                    close(fd_A[i]);
                    FD_CLR(fd_A[i],&fdsr);
                    fd_A[i] = 0;
                }else{
                    if(res < BUF_SIZE){
                        memset(&buff[res],'\0',1);
                        printf("client[%d] send:%s\n",i,buff);
                    }
                }
            }
        }
        
        if(FD_ISSET(sockfd,&fdsr)){
            newfd = accept(sockfd,(struct sockaddr*)&clientaddr,&clientlen);
            assert(newfd != -1);

            if(conn_count < 5){
                fd_A[conn_count++] = newfd;
                printf("new connection client[%d] %s:%d\n",conn_count,inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
                if(newfd > maxsock){
                    maxsock = newfd;
                }
            }else{
                printf("max connections arrive,exit\n");
                send(newfd,"bye",4,0);
                close(newfd);
                break;
            }
        }
        showclient();
    }

    for(i = 0; i<5;i++){
        if(fd_A[i] != 0){
            close(fd_A[i]);
            fd_A[i] = 0;
        }
    }
    exit(0);
}

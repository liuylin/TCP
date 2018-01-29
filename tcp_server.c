
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

static usage(const char* proc)
{
    printf("Usage:%s [local_ip][local_port]\n",proc);
}


int startup(const char* ip,int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);//IPV4   mianxiangliu
    if(sock < 0)
    {
        perror("socket\n");
        exit(2);
    }

    printf("sock:%d\n",sock);
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port =htons( port);
    local.sin_addr.s_addr = inet_addr(ip);

    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
        perror("bind");
        exit(3);
    }
    if(listen(sock,10) < 0)
    {
        perror("listen");
        exit(4);
    }
    return sock;
}

//./tcp_server 192.168.x.x 8080
int main(int argc,char*argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        return 1;
    }
    int listen_sock = startup(argv[1],atoi(argv[2]));

    while(1)
    {
       struct sockaddr_in client;
       socklen_t len = sizeof(client);
       int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len) ;
       if(new_sock < 0)
        {
            perror("accept\n");
            continue;
        }
        printf("get new client [%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
        //read -- write
        while(1)
        {
            char buf[1024];
            ssize_t s = read(new_sock,buf,sizeof(buf)-1);
            if(s>0)
            {
                buf[s] = 0;
                printf("client# %s\n",buf);
                write(new_sock,buf,strlen(buf));
            }
            else if(s == 0)
            {
                printf("client close!!\n");
                break;
            }
            else
            {
                perror("read\n");
                break;
            }
        }
        close(new_sock);
    }
    return 0;
}

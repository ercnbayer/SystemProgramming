
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h> 
#include <unistd.h>
#include <string.h>

#define QUEUE_LENGTH 10
#define CLIENT_LIMIT 10
#define  BYTE_LENGTH 1024
int g_TaskVariable=0;
pthread_mutex_t mutexLock;


void* ClientTask(void *clientSocketfd){
    int client_socket=*(int*)(clientSocketfd);
    char szRecvArr[BYTE_LENGTH+1]={0};

    int iMessageSize=recv(client_socket, szRecvArr,BYTE_LENGTH,0);// we could also do it with msg_peek
     while(iMessageSize){
        
        szRecvArr[iMessageSize]='\0';
        printf("%s",szRecvArr);
        if(iMessageSize <BYTE_LENGTH){
            break;
        }

        iMessageSize=recv(client_socket,szRecvArr,BYTE_LENGTH,0);

     }
    

    pthread_mutex_lock(&mutexLock);
    g_TaskVariable++;// it can overflow after INT_MAX TIME Request but at this example we dont care about it
    printf("global variable increased : %d\n ",g_TaskVariable);
    pthread_mutex_unlock(&mutexLock);
    
    shutdown(client_socket,SHUT_RDWR);
    close(client_socket);
}

void main(){

    int iServerSock;
    struct  sockaddr_in  TCPServerAdd;

    TCPServerAdd.sin_family = AF_INET;
    TCPServerAdd.sin_addr.s_addr =inet_addr("127.0.0.1");
    TCPServerAdd.sin_port = htons(7000);
    unsigned int  addressSize=sizeof(TCPServerAdd);

    //int iaClientSock[QUEUE_LENGTH];
    int iClientSock;
    iServerSock=socket(AF_INET,SOCK_STREAM,0);

    if(pthread_mutex_init(&mutexLock,0)!=0){
        perror("err mutex init");
        exit(1);
    }

    if(iServerSock<0){
        perror("socket init err");
        exit(1);
    }

    if(bind(iServerSock,(struct sockaddr *)&TCPServerAdd,addressSize)!=0){
        perror("bind err");
        exit(1);
    }

    listen(iServerSock,QUEUE_LENGTH);

    while(1){
        iClientSock=accept(iServerSock,(struct sockaddr*)&TCPServerAdd,(socklen_t*)&addressSize);
        
        pthread_t clientTh;

        if(pthread_create(&clientTh,NULL,ClientTask,(void*)&iClientSock)!=0){
            perror("thread create err");
            exit(1);
        }
    }

   shutdown(iServerSock,SHUT_RDWR);
   close(iServerSock);
   pthread_mutex_destroy(&mutexLock);

}
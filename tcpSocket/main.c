
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define QUEUE_LENGTH 10
#define CLIENT_LIMIT 100
pthread_t tasks[CLIENT_LIMIT];
int g_TaskVariable = 0;
pthread_mutex_t mutexLock;

void *ClientTask(void *clientSocketfd) {

  int client_socket = *(int *)(clientSocketfd);
  char szRecvArr[1025] = {0};
  int message_count = 0;

  while (1) {

    int iMessageSize = recv(client_socket, szRecvArr, 1024, 0); // setsockopt()
    // sleep(10);

    if (iMessageSize == 0) {

      perror("recv Sock err.Terminating Thread \n");
      shutdown(client_socket, SHUT_RDWR);

      close(client_socket);
      break;
    } 
    
    else if (iMessageSize> 0 ) {
  
      szRecvArr[iMessageSize] = '\0';

      int count=0;
      int iMessageCursor=0;

      while (szRecvArr[iMessageCursor]!=0) {
       
       if (szRecvArr[iMessageCursor] == '\n') {
        count++;
       }
        iMessageCursor++;
      }
      printf(" message_count:%d\n",count);
      printf("%s end \n", szRecvArr);

      pthread_mutex_lock(&mutexLock);

      g_TaskVariable+=count;
  
      printf("Task VAR:%d\n", g_TaskVariable);

      pthread_mutex_unlock(&mutexLock);
    }

  }

  free(clientSocketfd);
  return NULL;
}

void main() {

  int iServerSock;
  struct sockaddr_in TCPServerAdd;

  TCPServerAdd.sin_family = AF_INET;
  TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
  TCPServerAdd.sin_port = htons(5000);
  unsigned int addressSize = sizeof(TCPServerAdd);
  int iSetSockOptions = 1;
  int iaClientSock[QUEUE_LENGTH] = {0};
  int *iClientSock;
  iServerSock = socket(AF_INET, SOCK_STREAM, 0);

  if (setsockopt(iServerSock, SOL_SOCKET, SO_REUSEADDR, &iSetSockOptions,
                 sizeof(iSetSockOptions)) < 0) {
    perror("Something went wrong during port attach");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(iServerSock, SOL_SOCKET, SO_REUSEPORT, &iSetSockOptions,
                 sizeof(iSetSockOptions)) < 0) {
    perror("Something went wrong during port attach");
    exit(EXIT_FAILURE);
  }

  if (pthread_mutex_init(&mutexLock, 0) != 0) {
    perror("err mutex init");
    exit(1);
  }

  if (iServerSock < 0) {
    perror("socket init err");
    exit(1);
  }

  if (bind(iServerSock, (struct sockaddr *)&TCPServerAdd, addressSize) != 0) {
    perror("bind err");
    exit(1);
  }

  if (listen(iServerSock, QUEUE_LENGTH) < 0) {
    printf("err");
    exit(1);
  }
  int index = 0;
  while (1) {

    iClientSock = (int *)malloc(sizeof(int));
    if (iClientSock == NULL) {
      perror("malloc err\n");
      exit(EXIT_FAILURE);
    }

    (*iClientSock) = accept(iServerSock, (struct sockaddr *)&TCPServerAdd,
                            (socklen_t *)&addressSize);

    if (pthread_create(&tasks[index], NULL, ClientTask, (void *)iClientSock) !=
        0) {
      perror("thread create err");
      exit(1);
    }
  }
  // printf("glob var:%d\n",g_TaskVariable);
  shutdown(iServerSock, SHUT_RDWR);
  close(iServerSock);
  pthread_mutex_destroy(&mutexLock);
}
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define THREAD_LIMIT 10

int iaIndexes[THREAD_LIMIT];

int connectSocket() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    perror("socket could not created!\n");
    return -1;
  }

  printf("socket created!\n");

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(5000);

  if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) < 1) {
    perror("address invalid or not supported!\n");
    return -1;
  }

  int connectionStatus =
      connect(socketfd, (struct sockaddr *)&address, sizeof(address));
  if (connectionStatus < 0) {
    perror("connection not established!\n");
    return -1;
  }
  return socketfd;
}

void *sendMessageTask(void *args) {
  int iSocketIndex = *((int *)args);
  int socketfd = connectSocket();
  char szMessage[1024] = {0};
  for (int i = 0; i < 10; i++) {
    sprintf(szMessage, "Hello, this is message %d from Client %d!\n", i,
            iSocketIndex);
    int if_send = send(socketfd, szMessage, strlen(szMessage), 0);
    if (if_send == -1) {
      perror("err send");
      exit(-1);
    }
    printf("message sent!\n");
  }
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd);

  return NULL;
}

int main() {
  pthread_t tasks[THREAD_LIMIT] = {0};

  printf("client sockets starting...\n");
  for (int i = 0; i < THREAD_LIMIT; i++) {
    iaIndexes[i] = i + 1;
    if (pthread_create(&tasks[i], NULL, &sendMessageTask,
                       (void *)(&iaIndexes[i])) < 0) {
      perror("thread cannot created!\n");
      return -1;
    }
  }

  for (int i = 0; i <1; i++) {
    pthread_join(tasks[i], NULL);
  }

  printf("All messages sent!\n");

  return 0;
}
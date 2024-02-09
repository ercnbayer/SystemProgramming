#include <pthread.h>

#include <semaphore.h>

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <unistd.h>

// goal: for every entry in list which is on ram them carry it to disk (write to
// file) with order using sobjs and threads
typedef struct Entity {
  int id;
  struct Entity * next;
  struct Entity * tail;
}
Entity;

Entity * pRoot;
Entity *pTail;

void AddToTheTail(int id) { // creating entry here

  Entity * add = (Entity * ) malloc(sizeof(Entity));

  add -> id = id;
  add -> next = NULL;
  add -> tail = add;

  if (pRoot == NULL) {
    pRoot = add;
  }
  pRoot -> tail -> next = add;
  pRoot -> tail = pRoot -> tail -> next;
}
sem_t lock;

int iActiveTask = 0;

void * threadFn() {

  sem_wait( & lock);

  iActiveTask += 1;

  AddToTheTail(iActiveTask);

  sem_post( & lock);

  return NULL;
}

void WriteToFile() {

  FILE * pFile = fopen("list.cdt", "a");

  if (pFile) {

    Entity * pStart = pRoot;

    while (pStart) {

      fwrite(pStart, sizeof( * pStart), 1, pFile);

      printf("id:%d\n", pStart -> id);
      pStart = pStart -> next;
    }
    fclose(pFile);
  } else {

    perror("cant open file");
  }
}

void * writerTask() {

  sem_wait( & lock);
  printf("writing to file \n");

  if (iActiveTask >= 10) {

    WriteToFile();
    iActiveTask = 1;
    return NULL;
  }

  sem_post( & lock);
}

void main() {

  if (sem_init( & lock, 0, 1)) {
    perror("err init sem\n");
  }

  pthread_t threads[10];
  pthread_t fileth;
  for (int i = 0; i < 10; i++) {

    if (pthread_create( & threads[i], NULL, threadFn, NULL)) {
      perror("th create err\n");
    }
  }
  if (pthread_create( & fileth, NULL, writerTask, NULL)) {
    perror("disk writer err");
  }

  pthread_join(fileth, NULL);

  sem_destroy( & lock);

  while (pRoot) {
    Entity * pTemp = pRoot;
    pRoot = pRoot -> next;
    printf(" Freeing:%d \n", pTemp -> id);
    free(pTemp);
  }
}
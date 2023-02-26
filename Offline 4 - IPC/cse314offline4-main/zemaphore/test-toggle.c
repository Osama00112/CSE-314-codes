#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10

zem_t zemaphores[NUM_THREADS];  // for ordering of child threads
zem_t parent;                   // for backgroud thread
static int round = 0;

void *justprint(void *data)
{
  int thread_id = *((int *)data);
  for(int i=0; i < NUM_ITER; i++)
    {
      zem_down(&zemaphores[thread_id]);
      printf("This is thread %d\n", thread_id);
      zem_up(&parent);
    }
  return 0;
}

int main(int argc, char *argv[])
{
  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];

  for(int i=0; i < NUM_THREADS; i++){
      zem_init(&zemaphores[i], 0);
    }


  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }

  for(int i=0; i<NUM_ITER; i++){
    for(int j=0; j<NUM_THREADS; j++){
      zem_up(&zemaphores[j]);
      zem_down(&parent);
    }
  }

  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}

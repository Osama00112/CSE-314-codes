#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include "zemaphore.h"

#define TOTAL_OXYGEN 6
#define TOTAL_HYDROGEN 12

zem_t Hydro_queue;
zem_t Oxy_queue;
zem_t mutex_zem;
zem_t Hydro_barrier;
zem_t Oxy_barrier;
zem_t barrier_lock;

int curr_H2_count;
int curr_O2_count;
int bond_molecule_count;

void waitForBond(int type, int id){
    if(type){
        printf("1 Oxygen   with id %d is in queue\n", id);
    }else{
        printf("1 Hydrogen with id %d is in queue\n", id);
    }
}
void BondCompleted(int type, int id){
    if(type){
        printf("1 Oxygen   with id %d is forming a bond\n", id);
    }else{
        printf("1 Hydrogen with id %d is forming a bond\n", id);
    }
}

// oxygen func
void *bondWithH2(void *data){
    zem_down(&mutex_zem);
    int thread_id = *((int *)data);
    waitForBond(1, thread_id);

    curr_O2_count ++;
    if(curr_H2_count >= 2){
        zem_down(&barrier_lock);
        bond_molecule_count ++;

        zem_up(&Hydro_queue);
        zem_up(&Hydro_queue);
        curr_H2_count -= 2;
        zem_up(&Oxy_queue);
        curr_O2_count --;
    }else{
        zem_up(&mutex_zem);
    }
    
    zem_down(&Oxy_queue);
    BondCompleted(1, thread_id);
    bond_molecule_count ++;
    if(bond_molecule_count == 3){
        bond_molecule_count = 0;
        zem_up(&barrier_lock);
    }
    zem_up(&mutex_zem);
    return 0;
}

// hydrigen func
void *bondWithO2(void *data){
    zem_down(&mutex_zem);
    int thread_id = *((int *)data);
    waitForBond(0, thread_id);
    
    curr_H2_count ++;
    if(curr_H2_count >= 2 && curr_O2_count >= 1){
        zem_down(&barrier_lock);
        bond_molecule_count ++;
        zem_up(&Hydro_queue);
        zem_up(&Hydro_queue);
        curr_H2_count -= 2;
        zem_up(&Oxy_queue);
        curr_O2_count --;
    }else{
        zem_up(&mutex_zem);
    }
    
    zem_down(&Hydro_queue);
    BondCompleted(0, thread_id);
    bond_molecule_count ++;
    if(bond_molecule_count == 3){
        bond_molecule_count = 0;
        zem_up(&barrier_lock);
    }
    zem_up(&mutex_zem);
    return 0;
}


int main(int argc, char *argv[])
{

    zem_init(&mutex_zem, 1);
    zem_init(&barrier_lock, 1);
    zem_init(&Hydro_queue, 0);
    zem_init(&Oxy_queue, 0);

    curr_H2_count = 0;
    curr_O2_count = 0;
    bond_molecule_count = 0;
    
    int *H2_thread_id;
    pthread_t *H2_thread;

    int *O2_thread_id;
    pthread_t *O2_thread;
  
    int i;

    //create H2 threads
    H2_thread_id = (int *)malloc(sizeof(int) * TOTAL_HYDROGEN);
    H2_thread = (pthread_t *)malloc(sizeof(pthread_t) * TOTAL_HYDROGEN);
    for (i = 0; i < TOTAL_HYDROGEN; i++)
        H2_thread_id[i] = i;

    for (i = 0; i < TOTAL_HYDROGEN; i++)
        pthread_create(&H2_thread[i], NULL, bondWithO2, (void *)&H2_thread_id[i]);

    //create O2 threads
    O2_thread_id = (int *)malloc(sizeof(int) * TOTAL_OXYGEN);
    O2_thread = (pthread_t *)malloc(sizeof(pthread_t) * TOTAL_OXYGEN);
    for (i = 0; i < TOTAL_OXYGEN; i++)
        O2_thread_id[i] = i;

    for (i = 0; i < TOTAL_OXYGEN; i++)
        pthread_create(&O2_thread[i], NULL, bondWithH2, (void *)&O2_thread_id[i]);

    //wait for all threads to complete
    for (i = 0; i < TOTAL_HYDROGEN; i++)
    {
        pthread_join(H2_thread[i], NULL);
    }

    for (i = 0; i < TOTAL_OXYGEN; i++)
    {
        pthread_join(O2_thread[i], NULL);
    }

    //free(buffer);
    free(O2_thread_id);
    free(O2_thread);
    free(H2_thread_id);
    free(H2_thread);
    
    return 0;
}

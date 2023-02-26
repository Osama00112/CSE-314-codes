#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TOTAL_OXYGEN 6
#define TOTAL_HYDROGEN 12

pthread_mutex_t mutex1;
pthread_mutex_t barrier;
pthread_cond_t oxygenArrived, hydrogenArrived;

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
    pthread_mutex_lock(&mutex1);
    int thread_id = *((int *)data);
    waitForBond(1, thread_id);

    curr_O2_count ++;
    if(curr_H2_count >= 2){
        pthread_mutex_lock(&barrier);
        bond_molecule_count ++;
        curr_H2_count -= 2;
        curr_O2_count --;
        pthread_cond_signal(&hydrogenArrived);
        pthread_cond_signal(&hydrogenArrived);
        pthread_cond_signal(&oxygenArrived);
    }else{
        pthread_cond_wait(&oxygenArrived, &mutex1);
    }
    
    BondCompleted(1, thread_id);
    bond_molecule_count ++;
    if(bond_molecule_count == 3){
        bond_molecule_count = 0;
        pthread_mutex_unlock(&barrier);
    }
    pthread_mutex_unlock(&mutex1);
    return 0;
}

// hydrigen func
void *bondWithO2(void *data){
    pthread_mutex_lock(&mutex1);
    int thread_id = *((int *)data);
    waitForBond(0, thread_id);
    
    curr_H2_count ++;
    if(curr_H2_count >= 2 && curr_O2_count >= 1){
        pthread_mutex_lock(&barrier);
        bond_molecule_count ++;
        curr_H2_count -= 2;
        curr_O2_count --;
        pthread_cond_signal(&hydrogenArrived);
        pthread_cond_signal(&hydrogenArrived);
        pthread_cond_signal(&oxygenArrived);
    }else{
        pthread_cond_wait(&hydrogenArrived, &mutex1);
    }
    
    BondCompleted(0, thread_id);
    bond_molecule_count ++;
    if(bond_molecule_count == 3){
        bond_molecule_count = 0;
        pthread_mutex_unlock(&barrier);
    }
    pthread_mutex_unlock(&mutex1);
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&barrier, NULL);
    pthread_cond_init(&oxygenArrived, NULL);
    pthread_cond_init(&hydrogenArrived, NULL);

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
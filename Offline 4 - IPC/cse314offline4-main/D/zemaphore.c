#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    s->value = value;
    pthread_mutex_init(&s->zem_mutex, NULL);
    pthread_cond_init(&s->zem_cond, NULL);
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&s->zem_mutex);
    while(s->value <= 0){
        pthread_cond_wait(&s->zem_cond,&s->zem_mutex);
    };
    s->value --;
    pthread_mutex_unlock(&s->zem_mutex);
}

void zem_up(zem_t *s) {
    pthread_mutex_lock(&s->zem_mutex);
    s->value++;
    pthread_cond_signal(&s->zem_cond);
    pthread_mutex_unlock(&s->zem_mutex);
}

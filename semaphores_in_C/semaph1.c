#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define LOOP 10000
/*
*   functions to use 
*   int sem_init(sem_t *sem, int pshared, unsigned int value);
*   int sem_wait(sem_t *sem);
*   int sem_post(sem_t *sem);
*/

//UPDATE: this are UNNAMED SEMAPHORES!!!

void * functionINC(void * arg);
void * functionDEC(void * arg);

int counter = 0;

sem_t sem1;

int main(void){
    pthread_t th1, th2;

    sem_init(&sem1, 0, 1);

    pthread_create(&th1, NULL, functionINC, NULL);
    pthread_create(&th1, NULL, functionDEC, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    printf("counter = %d\n",counter);
    return 0;
}

void * functionINC(void * arg){
    for (int i = 0; i < LOOP; i++){
        sem_wait(&sem1);
        counter++;
        sem_post(&sem1);
    }
}

void * functionDEC(void * arg){
    for (int i = 0; i < LOOP; i++){
        sem_wait(&sem1);
        counter--;
        sem_post(&sem1);
    }
    
}
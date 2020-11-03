#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define LOOP 10000
#define SEM_NAME "/mysemaphore"



void * functionINC(void * arg);
void * functionDEC(void * arg);

int counter = 0;
sem_t *sem;

int main(void){
    sem_unlink(SEM_NAME);

    sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED){
        perror("sem_open/producer");
        exit(EXIT_FAILURE);
    }

    pthread_t th1, th2;

    pthread_create(&th1, NULL, functionINC, NULL);
    pthread_create(&th1, NULL, functionDEC, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    printf("counter = %d\n",counter);
    
    return 0;
}

void * functionINC(void * arg){
    for (int i = 0; i < LOOP; i++){
        sem_wait(sem);
        counter++;
        sem_post(sem);
    }
}

void * functionDEC(void * arg){
    for (int i = 0; i < LOOP; i++){
        sem_wait(sem);
        counter--;
        sem_post(sem);
    }
    
}
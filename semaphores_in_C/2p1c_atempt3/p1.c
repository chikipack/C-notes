#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

sem_t *sem_prod1;
sem_t *sem_cons;
sem_t *sem_prod2;

void * producir(){
    sem_prod1 = sem_open("/semaphA",0);
    sem_cons = sem_open("/semaphB",0);
    sem_prod2 = sem_open("/semaphC",0);

    while (1){
        sem_wait(sem_prod1);
        printf("produciendo en P1\n");
        sleep(1);
        sem_post(sem_cons);
    }
    
}

void * consumir(){
    sem_prod1 = sem_open("/semaphA",0);
    sem_cons = sem_open("/semaphB",0);

    while (1){
        sem_wait(sem_cons);
        printf("consumiendo P1\n");
        sleep(1);
        sem_post(sem_prod1);
    }

}

void * producir2(){
    sem_prod2 = sem_open("/semaphC",0);
    sem_cons = sem_open("/semaphB",0);

    while (1){
        sem_wait(sem_prod2);
        printf("\tproduciendo en P2\n");
        sleep(1);
        sem_post(sem_cons);
    }
    
}

void * consumir2(){
    sem_prod2 = sem_open("/semaphC",0);
    sem_cons = sem_open("/semaphB",0);

    while (1){
        sem_wait(sem_cons);
        printf("\tConsumiendo P2\n");
        sleep(1);
        sem_post(sem_prod2);
    }

}

int main(){
    


    pthread_t productor, consumidor;
    pthread_t productor2, consumidor2;

    pthread_create(&productor, NULL, producir, NULL);
    pthread_create(&consumidor, NULL, consumir, NULL);

    pthread_create(&productor2, NULL, producir2, NULL);
    pthread_create(&consumidor2, NULL, consumir2, NULL);

    ////////////////////////////////////////////////
    
    pthread_join(productor, NULL);
    pthread_join(consumidor, NULL);

    pthread_join(productor2, NULL);
    pthread_join(consumidor2, NULL);

}
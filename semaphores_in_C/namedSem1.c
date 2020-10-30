#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOOP 10000

void * functionINC(void * arg);
void * functionDEC(void * arg);

int counter = 0;

sem_t *my_semaphore;



int main(void){
    pthread_t th1, th2;
    my_semaphore = sem_open("/mysemaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
    //sem_init(&sem1, 0, 1);

    pthread_create(&th1, NULL, functionINC, NULL);
    pthread_create(&th1, NULL, functionDEC, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    printf("counter = %d\n",counter);
    return 0;
}

void * functionINC(void * arg){
    for (int i = 0; i < LOOP; i++){
        sem_wait(my_semaphore);
        counter++;
        sem_post(my_semaphore);
    }
}

void * functionDEC(void * arg){
    for (int i = 0; i < LOOP; i++){
        sem_wait(my_semaphore);
        counter--;
        sem_post(my_semaphore);
    }
    
}
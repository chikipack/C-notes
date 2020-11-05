#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

int main(void){
    sem_t * semA;
    sem_t * semB;

    semA = sem_open("/semaphA",0);
    semB = sem_open("/semaphB",0);

    while (1)
    {
        sem_wait(semB);
        printf("Proceso B \n");
        sleep(1);
        sem_post(semA);
    }
    
}
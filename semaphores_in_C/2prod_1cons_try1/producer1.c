#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

int main(void){
    sem_t * semA;
    sem_t * semB;

    semA = sem_open("/semaphA",0);
    semB = sem_open("/semaphB",0);
    int count=1;

    while (1)
    {
        sem_wait(semA);
        printf("B atiende Proceso A por %d vez\n",count);
        sleep(1);
        count++;
        sem_post(semB);
    }
    
}
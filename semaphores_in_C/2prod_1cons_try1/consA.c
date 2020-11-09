#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

int main(void){
    sem_t * semA;
    sem_t * semB;

    semA = sem_open("/semaphA",0);
    semB = sem_open("/semaphB",0);
    int counter = 1;
    while (1)
    {
        sem_wait(semB);
        printf("Proceso B libera A por %d vez \n",counter);
        sleep(1);
        counter++;
        sem_post(semA);
    }
    
}
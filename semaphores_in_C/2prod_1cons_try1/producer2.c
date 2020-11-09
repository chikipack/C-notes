#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

int main(void){
    sem_t * semC;
    sem_t * semB;

    semC = sem_open("/semaphC",0);
    semB = sem_open("/semaphB",0);
    int counter = 1;

    while (1)
    {
        sem_wait(semC);
        printf("B atiende Proceso C por %d vez \n",counter);
        sleep(1);
        counter++;
        sem_post(semB);
    }
    
}
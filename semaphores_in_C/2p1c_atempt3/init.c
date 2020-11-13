#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEMAPH_NAME_A "/semaphA"
#define SEMAPH_NAME_B "/semaphB"
#define SEMAPH_NAME_C "/semaphC"

int main(void){
    //sem_open returns a data type sem_t * 
    sem_t *semA;
    sem_t *semB;
    sem_t *semC;

    sem_unlink(SEMAPH_NAME_A);
    sem_unlink(SEMAPH_NAME_B);
    sem_unlink(SEMAPH_NAME_C);

    int initial_value_semA;
    int initial_value_semB;
    int initial_value_semC;

    semA = sem_open(SEMAPH_NAME_A, O_CREAT | O_EXCL, 0644, 1);
    semC = sem_open(SEMAPH_NAME_C, O_CREAT | O_EXCL, 0644, 0);
    semB = sem_open(SEMAPH_NAME_B, O_CREAT | O_EXCL, 0644, 0);

    //sem_getvalue regresa un entero del valor inicial del semaforo
    sem_getvalue(semA, &initial_value_semA);
    sem_getvalue(semB, &initial_value_semB);
    sem_getvalue(semC, &initial_value_semC);

    printf("el valor inicial del semA es: %d \n",initial_value_semA);
    printf("el valor inicial del semB es: %d \n",initial_value_semB);
    printf("el valor inicial del semC es: %d \n",initial_value_semC);

}
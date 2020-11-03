#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEMAPH_NAME_A "/semaphA"
#define SEMAPH_NAME_B "/semaphB"

int main(void){
    //sem_open returns a data type sem_t * 
    sem_t *semA;
    sem_t *semB;

    sem_unlink(SEMAPH_NAME_A);
    sem_unlink(SEMAPH_NAME_B);

    int initial_value_semA;
    int initial_value_semB;
    //0644 means:
    //0 is for octan initiation
    /*
        then the next 3 numbers are for the permision to USER - GROUP - OTHER
                                                            6       4       4
        and we have that user has permision 6 wich is read & writ permision
        and group and other has permision 4 wich means just read permision
    */

    semA = sem_open(SEMAPH_NAME_A, O_CREAT | O_EXCL, 0644, 1);
    semB = sem_open(SEMAPH_NAME_B, O_CREAT | O_EXCL, 0644, 0);

    sem_getvalue(semA, &initial_value_semA);
    sem_getvalue(semB, &initial_value_semB);

    printf("el valor inicial del semA es: %d \n",initial_value_semA);
    printf("el valor inicial del semB es: %d \n",initial_value_semB);
}
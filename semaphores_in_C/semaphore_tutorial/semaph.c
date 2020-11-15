/*
All the credits to Soham Shah - https://medium.com/@sohamshah456/producer-consumer-programming-with-c-d0d47b8f103f
*/

//pensaba poner explicaciones en espaniol pero si estas estudiando programacion ya deberias de saber ingles, no mamarss bro #SorryNotSorry
//the Producer-Consumer Problem || Hello world Version || Named semaphores version

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>


#define SHARED 1
#define EMPTY "/empty"
#define FULL "/full"

//declaration of functions
void * producer();
void * consumer();

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

sem_t *empty, *full; //Sempahores to be used
sem_t amp, sm;
int data;

int main(){
    sem_unlink(EMPTY);
    sem_unlink(FULL);

    pthread_t ptid, ctid; // 
    printf("Main started, Bitch!\n");
    empty = sem_open(EMPTY, O_CREAT | O_EXCL, 0644, 1);
    full = sem_open(FULL, O_CREAT | O_EXCL, 0644, 0);

    pthread_create(&ptid, NULL, producer, NULL);
    pthread_create(&ctid, NULL, consumer, NULL);

    pthread_join(ptid, NULL);
    pthread_join(ctid, NULL);
    
    sem_unlink(EMPTY);
    sem_unlink(FULL);

    printf("Main Done!\n");
}

void * producer(){
    //ITEM PRODUCED...

    int produced;
    printf("\nProduced Created\n");
    printf("Produced id is %ld\n",pthread_self());//print thread id
    for(produced = 0; produced <100; produced++){
        sleep(1);
        sem_wait(empty);
            //LOCK Starts
           
                    //Critical Section STARTS
                        data = produced;
                    //END of Critical Section
               
            //LOCK ends
        sem_post(full);
        printf("Producer: %d\n",data);
    }
}

void * consumer(){
    int consumed, total=0;
    printf("Consumer created\n");
    printf("Consumer ID is: %ld\n",pthread_self());

    for (consumed = 0; consumed < 100; consumed++){
        sem_wait(full);
            //LOCK starts
                
                    //CRITICAL SECTION STARTS
                        total = total + data;
                    //CRITICAL SECTION ENDS
                
            //LOCK ends
        sem_post(empty);
        printf("Consumed: %d\n",data);
    }    
    printf("The total of 100 iterations is %d\n",total);
}
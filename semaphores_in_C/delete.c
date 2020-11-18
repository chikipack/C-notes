#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#define SHARED 1

void * producer();
void * consumer();

//sm y sm2 is the critical section semaphore
sem_t empty, full, sm,sm2,sm_cons, sm2_cons;

//secciones criticas
int data1, data2;

//funciones para acceder a la seccion criticas
void preguntar_consumidor();
void preguntar(int num);

int x=1, y=2, z=3;
int *a=&x;
int *b=&y;
int *c=&z;

int main(){

    pthread_t thread1, thread2,thread_test;
    pthread_t cons1, cons2;
    sem_init(&empty,SHARED,2);
    sem_init(&full,SHARED,0);
    sem_init(&sm,SHARED,1);
    sem_init(&sm2,SHARED,1);
    sem_init(&sm_cons, SHARED, 0);
    sem_init(&sm2_cons, SHARED, 0);
    
    pthread_create(&thread1, NULL, producer, (void*)a);
    pthread_create(&thread2, NULL, producer, (void*)b);
    //pthread_create(&thread_test, NULL, producer, (void*)c);
    //sleep(1);
    pthread_create(&cons1, NULL, consumer, (void*)a);
    pthread_create(&cons2, NULL, consumer, (void*)b);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    //pthread_join(thread_test,NULL);
    pthread_join(cons1,NULL);
    pthread_join(cons2,NULL);
 
    return 0;
}

//YA NOMAS FALTA PONER LOS PUTOS FOR Y YA ALV CHINGO ASUMADRE LA TAREAAA
void * producer(void * no){
    int produced=0;
    int *thread = (int*)no;
    printf("\nProducer %d Created---",*thread);
    printf("Producer id is %ld\n",pthread_self());
    for(produced=0;produced<3;produced++){
        sem_wait(&empty);
            preguntar(produced);
        sem_post(&full);
    }
    return NULL;
}

void * consumer(void * no){
    int consumed;
    int *thread = (int*)no;
    printf("\nConsumer %d created---",*thread);
    printf("Consumer id is %ld\n",pthread_self());
    for(consumed=0;consumed<3;consumed++){
        sem_wait(&full);
            preguntar_consumidor();
        sem_post(&empty);
    }
    return NULL;
}

void preguntar(int num){
    int numero=num;
    if (sem_trywait(&sm)==-1){
        printf("seccion critica 1 OCUPADA, intentando entrar a seccion critica 2\n");
        if (sem_trywait(&sm2)==-1){
            printf("seccion critica 2 OCUPADA, intentando entrar a seccion critica 1\n");
            //preguntar(numero);
        }else{
            printf("accediste a la seccion critica 2\n");
            data2 = num;
            printf("data2:%d\n",data2);
            sem_post(&sm2_cons);
            return NULL;
        }
    }else{
        printf("entraste en la seccion critica 1\n");
        data1 = num;
        printf("data1:%d\n",data1);
        sem_post(&sm_cons);
        return NULL;
    }
}

void preguntar_consumidor(){
    if (sem_trywait(&sm_cons)==-1){
        printf("no se puede Consumir lo que esta en la SC1, intentando consumir lo de la SC2\n");
        if(sem_trywait(&sm2_cons)==-1){
            printf("no se puede consumir lo de SC2, intentando consumir SC1\n");
            //preguntar_consumidor();
        }else{
            printf("Consumidor entro a la SC2\n");
            printf("Consumiendo:%d\n",data2);
            sem_post(&sm2);
            return NULL;
        }
    }else{
        printf("Consumidor entro a la SC1\n");
        printf("Consumiendo:%d\n",data1);
        sem_post(&sm);
        return NULL;
    }   
}
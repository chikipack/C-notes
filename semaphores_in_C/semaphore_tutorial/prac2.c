/*
4 productores
10,000 producciones por cada productor
40,000 producciones en total
productor 1 produce 1111
...
productor 4 produce 4444

6 secciones criticas

3 consumidores 
los consumidores deben leer el consumo y guardarlo en un archivo segun su tipo
ejemplo: si el consumidor 1 consume 1111 lo guarda en un archivo solo con los 1111
si se consume 2222 se debe guardar en un archivo unicamente con 2222
al final debe haver 4 archivos con 10,000 lineas cada uno
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#define SHARED 1
#define PRODUCCIONES 10000

void * producer();//funcion productora
void * consumer();//funcion consumidora

void preguntar_consumidor();
void preguntar(int num);

int data1, data2, data3, data4, data5, data6; //secciones criticas

sem_t in_main_section, out_main_section;
sem_t crit_sec1, crit_sec1cons;
sem_t crit_sec2, crit_sec2cons;
sem_t crit_sec3, crit_sec3cons;
sem_t crit_sec4, crit_sec4cons;
sem_t crit_sec5, crit_sec5cons;
sem_t crit_sec6, crit_sec6cons;

int w = 1, x=2, y=3, z=4;
int *a=&w;
int *b=&x;
int *c=&y;
int *d=&z;

int main(){
    pthread_t prod1, prod2, prod3, prod4;
}

void * producer(void * no){
    int produced=0;
    int *thread = (int*)no;
    printf("\nProducer %d Created---",*thread);
    printf("Producer id is %ld\n",pthread_self());
    for(produced=0;produced<30;produced++){
        sem_wait(&empty);
            preguntar(produced);
        sem_post(&full);
    }
    return NULL;
}
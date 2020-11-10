#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SEM_PRODUCER_NAME "/myproducer"
#define SEM_PRODUCER_NAME_2 "/myproducer2"
#define SEM_CONSUMER_NAME "/myconsumer"

void * produce();
void * produce_2();
void * consume();
void * consume_2();
int varCrit;
sem_t *sem_prod;

sem_t *sem_prod_2;

sem_t *sem_cons;


int main()
{
  sem_prod = sem_open(SEM_PRODUCER_NAME, O_CREAT, S_IRUSR | S_IWUSR,0);
  sem_cons = sem_open(SEM_CONSUMER_NAME, O_CREAT, S_IRUSR | S_IWUSR,1);
  sem_prod_2 = sem_open(SEM_PRODUCER_NAME_2, O_CREAT, S_IRUSR | S_IWUSR,0);
  pthread_t prod, prod_2,cons, cons_2;
  //1.-Crear el semáforo, validar que se creó correctamente
  if(sem_prod == SEM_FAILED){
    printf("Error al crear el semáforo\n");
    exit(-1);
  }

  if (sem_cons  == SEM_FAILED){
    printf("Error al crear el semáforo\n");
    exit(-1);
  }
  

  //2.- Crear los hilos
  pthread_create(&prod,NULL,(void*)produce,NULL);
  pthread_create(&prod_2,NULL,(void*)produce_2,NULL);
  pthread_create(&cons,NULL,(void*)consume,NULL);
  pthread_create(&cons_2,NULL,(void*)consume_2,NULL);

  pthread_join(prod,NULL);
  pthread_join(prod_2,NULL);
  pthread_join(cons,NULL);
  pthread_join(cons_2,NULL);

  //5.-Eliminar el semáforo
  sem_close(sem_prod);
  sem_close(sem_prod_2);
  sem_close(sem_cons);
  sem_unlink(SEM_PRODUCER_NAME);
  sem_unlink(SEM_PRODUCER_NAME_2);
  sem_unlink(SEM_CONSUMER_NAME);

  return 0;
}

//3.- Crear las funciones / Productor: genera valores enteros del 1 al 20
void* produce(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(sem_cons);
    varCrit=i;
    printf("produciendo en prod 1: %d\n",varCrit);
    sem_post(sem_prod);
  }
  
}

void* produce_2(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(sem_cons);
    varCrit=i;
    printf("produciendo en prod 2: %d\n",varCrit);
    sem_post(sem_prod_2);
  }
  
}

void * consume(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(sem_prod);
    printf("consumiendo lo de prod1: %d\n",varCrit);
    sem_post(sem_cons);
  }
  
}

void * consume_2(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(sem_prod_2);
    printf("consumiendo lo de prod2: %d\n",varCrit);
    sem_post(sem_cons);
  }
  
}
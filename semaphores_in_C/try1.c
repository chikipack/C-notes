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
#define SEM_CONSUMER_NAME "/myconsumer"

void * produce();
void * consume();
int varCrit;
sem_t *sem_prod;

sem_t *sem_cons;


int main()
{
  sem_prod = sem_open(SEM_PRODUCER_NAME, O_CREAT, S_IRUSR | S_IWUSR,0);
  sem_cons = sem_open(SEM_CONSUMER_NAME, O_CREAT, S_IRUSR | S_IWUSR,1);
  pthread_t prod,cons;
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
  sleep(3);
  pthread_create(&cons,NULL,(void*)consume,NULL);

  pthread_join(prod,NULL);
  pthread_join(cons,NULL);

  //5.-Eliminar el semáforo
  sem_close(sem_prod);
  sem_unlink(SEM_PRODUCER_NAME);
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
    printf("produciendo: %d\n",varCrit);
    sem_post(sem_prod);
  }
  
}

void * consume(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(sem_prod);
    printf("consumiendo: %d\n",varCrit);
    sem_post(sem_cons);
  }
  
}
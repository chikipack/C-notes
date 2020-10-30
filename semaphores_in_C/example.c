#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
void * produce();
void * consume();
int varCrit;
sem_t sem,sem2;

int main()
{
  int err;
  pthread_t prod,cons;
  //1.-Crear el semáforo, validar que se creó correctamente
  if(sem_init(&sem,0,0)<0 || sem_init(&sem2,0,1)<0){
    printf("Error al crear el semáforo\n");
    exit(-1);
  }

  //2.- Crear los hilos
  pthread_create(&prod,NULL,(void*)produce,NULL);
  pthread_create(&cons,NULL,(void*)consume,NULL);

  pthread_join(prod,NULL);
  pthread_join(cons,NULL);

  //5.-Eliminar el semáforo
  sem_destroy(&sem);
  return 0;
}

//3.- Crear las funciones / Productor: genera valores enteros del 1 al 20
void* produce(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(&sem2);
    varCrit=i;
    printf("produciendo: %d\n",varCrit);
    sem_post(&sem);
  }
  
}

void * consume(){
  for (int i = 1; i < 21; i++)
  {
    //4.-Administrar el semáforo
    sem_wait(&sem);
    printf("consumiendo: %d\n",varCrit);
    sem_post(&sem2);
  }
  
}
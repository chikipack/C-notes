#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define SEM_PRODUCER_NAME "/myproducer"
#define SEM_CONSUMER_NAME "/myconsumer"

typedef struct{
    int id;
    int l;
    int h;
}pair;

sem_t *sem_prod1;
sem_t *sem_cons1;
sem_t *sem_prod2;
sem_t *sem_cons2;

int varCrit[2],n;

void *producer(void *args){

  //Rangos a producir
  pair *pr=(pair*)args;
  int l = pr->l;
  int h = pr->h;

  for (int i = l; i <=h; i++){
    //Vamos a crear un ciclo para recorrer las secciones criticas
    for(int j=0;j<2;j++){
      
      if(j==0){
        //Esta parte es la sincronizacion de la seccion critica 1
        sem_wait(sem_prod1);
        varCrit[j]=i;//<-esto representa la seccion critica 1
        printf("produciendo en seccion 1: %d\n",varCrit[j]);
        sem_post(sem_cons1);  

      }else{
        //Esta parte es la sincronizacion de la seccion critica 2
        sem_wait(sem_prod2);
        varCrit[j]=i;//<-esto representa la seccion critica 2
        printf("produciendo en seccion 2: %d\n",varCrit[j]);
        sem_post(sem_cons2); 
      }
      
    }
  } 
}

void *consumer(void *args){
  for (int i = 1; i <= n; i++){

    for(int j=0;j<2;j++){  
      if(j==0){

        sem_wait(sem_cons1);
        printf("consumiendo en seccion 1: %d\n",varCrit[j]);
        sem_post(sem_prod1);  

      }else{

        sem_wait(sem_cons2);
        printf("consumiendo en seccion 2: %d\n",varCrit[j]);
        sem_post(sem_prod2); 
      }
      
    }
  }
}


int main(int argc,char  *argv[]){

  int nprod=atoi(argv[1]);
  int ncons=atoi(argv[2]);
  n        =atoi(argv[3]);

  /*Estos dos sem son para la primera seccion critica*/

  sem_prod1 = sem_open(SEM_PRODUCER_NAME, O_CREAT, S_IRUSR | S_IWUSR,1);
  sem_cons1 = sem_open(SEM_CONSUMER_NAME, O_CREAT, S_IRUSR | S_IWUSR,0);
  
  /*Estos dos sem son para la segunda seccion critica*/

  sem_prod2 = sem_open(SEM_PRODUCER_NAME, O_CREAT, S_IRUSR | S_IWUSR,1);
  sem_cons2 = sem_open(SEM_CONSUMER_NAME, O_CREAT, S_IRUSR | S_IWUSR,0);

  if(sem_prod1 == SEM_FAILED){
    printf("Error al crear el semáforo\n");
    exit(-1);
  }

  if (sem_cons1  == SEM_FAILED){
    printf("Error al crear el semáforo\n");
    exit(-1);
  }

  pthread_t *prodth,*consth;
  prodth = (pthread_t*)malloc(sizeof(pthread_t*)*nprod);
  consth = (pthread_t*)malloc(sizeof(pthread_t*)*ncons);

  pair * pairs;
  pairs = (pair*)malloc(sizeof(pair)*nprod);

  //Parametros para definir el numero de elementos a producir por cada productor
  int c=n/nprod,r=n%nprod,l=0,h=0;

  //Creacion hilos productor

  for(int i=0;i<nprod;i++){
    if(i==0){
      l=1;
      h+=(c+r);
    }else{
      l=h+1;
      h+=c;
    }
    pairs[i].l=l;
    pairs[i].h=h;
    pairs[i].id=i+1;
    pthread_create(&prodth[i], NULL,(void*)producer,(void*)&pairs[i]);
  }
  
  //Creacion hilos consumidor
  for(int i=0;i<ncons;i++){
    pthread_create(&consth[i],NULL,(void*)consumer,NULL);
  }

   //Creacion hilos productor
  for(int i=0;i<nprod;i++){
    pthread_join(prodth[i],NULL);   
  } 
    
  for(int i=0;i<ncons;i++){
    pthread_join(consth[i],NULL);
  }

  sem_close(sem_prod1);
  sem_close(sem_cons1);
  sem_close(sem_prod2);
  sem_close(sem_cons2);


  return 0;
}


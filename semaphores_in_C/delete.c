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

/*
La funcion Producer lo que hace es 
1.- notifica la creacion del hilo e imprime su identificador
2.-crea un ciclo con N iteraciones que son las producciones a realizar
3.- dentro del ciclo for se bloquea el primer semaforo (empty)
este primer semaforo tiene un valor inicial de 2, por lo que pueden entrar dos hilos productores al mismo tiempo
sin embargo, esta no es la seccion critica
4.- una vez bloqueado el semaforo empty manda a llamara la funcion preguntar() //ver en los comentarios de la seccion preguntar
5.-una vez finalazada la funcion preguntar desbloquea el semaforo full para que el consumidor pueda consumir

*/
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

/*
lo que hace la funcion consumer es lo siguiente:
1.-notifica la creacion del hilo consumidor e imprime su identificador
2.-crea un ciclo con n iteraciones que en teoria, deberian ser las mismas que las del productor para que pueda consumir las producciones
3.-se bloquea el semaforo full para que no puedan entrar mas de 2 consumidores al mismo tiempo
4.-se llama la funcion preguntar_consumido()//ver los comentarios de la funcion preguntar_consumidor()
5.-se libera el semaforo productor para que este siga produciendo
*/

void * consumer(void * no){
    int consumed;
    int *thread = (int*)no;
    printf("\nConsumer %d created---",*thread);
    printf("Consumer id is %ld\n",pthread_self());
    for(consumed=0;consumed<30;consumed++){
        sem_wait(&full);
            preguntar_consumidor();
        sem_post(&empty);
    }
    return NULL;
}

/*
la funcion preguntar a mi parecer es la mas importante ya que aqui es donde se lleva a acabo la magia de las secciones criticas
lo que se hace en esta funcion es:
    preguntar si puedo acceder a la seccion critica 1
    puedo entrar a la SC1?
        a.- si la respuesta es si: escribo el dato en la SC1 data1
        b.- si la respuesta es no: pregunto si puedo acceder a la SC2 data2
            I.-si la respues es si: escribo el dato en la SC2 data2
            II.- si la respuesta es no: se vuelve a llamar la funcion preguntar para preguntar si ya se puede usar la SC1
    ahora lo que hacen los semaforos en esta funcion es bloquear la seccion critica con semaforoz cruzados
    es decir
    se bloquea sm para que nadia mas pueda entrar a escribir
    se escribe 
    se desbloquea sm_cons para que se pueda consumir el dato 
    y se deja bloqueado para que nadie mas pueda entrar a escribir hasta que el dato sea consumido (ver funcion preguntar_consumidor() )
    esto sucede con ambas secciones criticas
*/

void preguntar(int num){
    int numero=num;
    if (sem_trywait(&sm)==-1){
        printf("seccion critica 1 OCUPADA, intentando entrar a seccion critica 2\n");
        if (sem_trywait(&sm2)==-1){
            printf("seccion critica 2 OCUPADA, intentando entrar a seccion critica 1\n");
            preguntar(numero);
        }else{
            printf("Productor entro a la seccion critica 2\n");
            data2 = num;
            printf("data2:%d\n",data2);
            sem_post(&sm2_cons);
            return NULL;
        }
    }else{
        printf("Productor entro en la seccion critica 1\n");
        data1 = num;
        printf("data1:%d\n",data1);
        sem_post(&sm_cons);
        return NULL;
    }
}

/*
la funcion preguntar consumidor hace lo mismo que la funcion preguntar
la diferencia es que lee el dato escrito en la seccion critica, lo imprime y libera la seccion critica
para que otro productor pueda entrar a sobreescribir el dato que ya no nos sirve porque ya fue leido
*/

void preguntar_consumidor(){
    if (sem_trywait(&sm_cons)==-1){
        printf("no se puede Consumir lo que esta en la SC1, intentando consumir lo de la SC2\n");
        if(sem_trywait(&sm2_cons)==-1){
            printf("no se puede consumir lo de SC2, intentando consumir SC1\n");
            preguntar_consumidor();
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
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#define SHARED 1
#define PRODUCCIONES 3

int value;

sem_t in_main_section, out_main_section;
sem_t crit_sec[6];
sem_t crit_sec_cons[6];

FILE * fichero1;
FILE * fichero2;

int w=1,x=2,y=3,z=4;
int *a=&w;
int *b=&x;
int *c=&y;
int *d=&z;

int data1, data2, data3, data4, data5, data6;

void crear_semaforos_CS();
void * producer();
void * consumer();
void producir();
void consumir();
void preguntar();
void preguntar_consumidor();

int main(){

    pthread_t prod1, prod2, prod3, prod4;   //4 productores
    pthread_t cons1, cons2, cons3;          //3 consumidores

    // inicializacion de semaforos
    crear_semaforos_CS();

    pthread_create(&prod1, NULL, producer,(void*)a);
    pthread_create(&prod2, NULL, producer,(void*)b);
    // pthread_create(&prod3, NULL, producer,(void*)c);
    // pthread_create(&prod4, NULL, producer,(void*)d);

    pthread_create(&cons1,NULL,consumer,(void*)a);
    
    pthread_join(prod1,NULL);
    pthread_join(prod2,NULL);
    // pthread_join(prod3,NULL);
    // pthread_join(prod4,NULL);

    pthread_join(cons1,NULL);

    
    return 0;
}

void * producer(void * no){
    int contador1111=1;
    int dato1=1111;

    int contador2222=1;
    int dato2=2222;

    int contador3333=1;
    int dato3=3333;
    
    int contador4444=1;
    int dato4=4444;

    int *thread = (int*)no;
    printf("Hi, i'm the producer %d - ID: %ld\n",*thread,pthread_self());
    switch (*thread){
        case 1:
        printf("voy a producir 1111\n");
        producir(contador1111,dato1);
        break;

        case 2:
        printf("voy a producir 2222\n");
        producir(contador2222,dato2);
        break;

        case 3:
        printf("voy a producir 3333\n");
        producir(contador3333,dato3);
        break;
        
        case 4:
        printf("voy a producir 4444\n");
        producir(contador4444,dato4);
    }
}

void producir(int contador, int dato){
    int count=contador;
    for(int i=0;i<PRODUCCIONES;i++){
        sem_wait(&in_main_section);
            preguntar(count, dato);
            //sem_getvalue(&in_main_section,&value);
            //printf("valor del semaforo: %d\n",value);
            //printf("produccion #%d\n",count);
            count++;
            // printf("dato: %d\n",dato);
        sem_post(&out_main_section);
    }

}

void * consumer(void * no){
    int *thread = (int*)no;
    printf("\nConsumer %d created-ID %d",*thread,pthread_self());
    //este for nomas es para ver que jale 
    //debemos de quitarlo y usar un while que pueda terminar de alguna forma
    
    for (int i = 0; i<6; i++){
        consumir();
    }
    

    // while(1){
    //     /*
    //     este ciclo va a estar activo 3 veces al mismo tiempo 
    //     solo hay que estar revisando las 6 secciones criticas, en algun momneto algun hilo va a poder consumir algo 
    //     en cada sc le ponemos la funcion y le madamos el dato
    //     */
    //    consumir();
       
    // }

    // for(int i=0;i<PRODUCCIONES;i++){
    //     sem_wait(&out_main_section);
    //         preguntar_consumidor();
    //     sem_post(&in_main_section);
    // }
}


void consumir(){
    sem_wait(&out_main_section);
        preguntar_consumidor();
    sem_post(&in_main_section);
}


void crear_semaforos_CS(){

    sem_init(&in_main_section, SHARED, 6);//empieza en 6 para poder acceder a las 6 secciones criticas
    sem_init(&out_main_section,SHARED,0);

    for(int i=0;i<6;i++){
        sem_init(&crit_sec[i],SHARED,1);
    }
    int value;
    // for (int i = 0; i < 6; i++){
    //     //sem_getvalue(&crit_sec[i],&value);
    //     //printf("%d\n",value);
    // }
    
    for(int i=0;i<6;i++){
        sem_init(&crit_sec_cons[i],SHARED,0);
    }

    int value2;
    // for (int i = 0; i < 6; i++){
    //     //sem_getvalue(&crit_sec_cons[i],&value2);
    //     //printf("%d\n",value2);
    // }
}

void preguntar(int contador, int dato){
    if (sem_trywait(&crit_sec[0])==-1){
        printf("seccion critica 1 OCUPADA, intentando entrar a seccion critica 2\n");
        if (sem_trywait(&crit_sec[1])==-1){
            printf("seccion critica 2 OCUPADA, intentando entrar a seccion critica 3\n");
            if(sem_trywait(&crit_sec[2])==-1){
                printf("seccion critica 3 ocupada, intentando entrar a la seccion critica 4\n");
                if(sem_trywait(&crit_sec[3])==-1){
                    printf("seccion critica 4 ocupada, intentando entrar a la seccion critica 5\n");
                    if(sem_trywait(&crit_sec[4])==-1){
                        printf("seccion critica 5 ocupada, intentando entrar a la seccion critica 6\n");
                        if(sem_trywait(&crit_sec[5])==-1){
                            printf("seccion critica 6 ocupada, intentando entrar a la seccion critica 1\n");
                            preguntar(contador, dato);
                        }else{
                            printf("Productor entro en la seccion critica 6\n");
                            data6 = dato;
                            sem_post(&crit_sec_cons[5]);
                            return;
                        }
                    }else{
                        printf("Productor entro en la seccion critica 5\n");
                        data5 = dato;
                        sem_post(&crit_sec_cons[4]);
                        return;
                    }
                }else{
                    printf("Productor entro en la seccion critica 4\n");
                    data4 = dato;
                    sem_post(&crit_sec_cons[3]);
                    return;
                }
            }else{
                printf("Productor entro en la seccion critica 3\n");
                data3 = dato;
                sem_post(&crit_sec_cons[2]);
                return;
            }
        }else{
            printf("Productor entro a la seccion critica 2\n");
            data2 = dato;
            sem_post(&crit_sec_cons[1]);
            return;
        }
    }else{
        printf("Productor entro en la seccion critica 1\n");
        data1 = dato;
        sem_post(&crit_sec_cons[0]);
        return;
    }
}

void preguntar_consumidor(){
    if (sem_trywait(&crit_sec_cons[0])==-1){
        printf("no se puede Consumir lo que esta en la SC1, intentando consumir lo de la SC2\n");
        if(sem_trywait(&crit_sec_cons[1])==-1){
            printf("no se puede consumir lo de SC2, intentando consumir SC3\n");
            if(sem_trywait(&crit_sec_cons[2])==-1){
                printf("no se puede consumir lo de SC3, intentando consumir SC4\n");
                if(sem_trywait(&crit_sec_cons[3])==-1){
                    printf("no se puede consumir lo de SC4, intentando consumir SC5\n");
                    if(sem_trywait(&crit_sec_cons[4])==-1){
                        printf("no se puede consumir lo de SC5, intentando consumir SC6\n");
                        if(sem_trywait(&crit_sec_cons[5])==-1){
                            printf("no se puede consumir lo de SC6, intentando consumir SC1\n");
                            preguntar_consumidor();
                        }else{
                            printf("se consumio SC6\n");
                            switch(data6){
                                case 1111:
                                printf("se guardo el dato 1111\n");
                                fichero1 = fopen("unos.txt","w");
                                fprintf(fichero1,"%d",data6);
                                break;

                               case 2222:
                                printf("se guardo el dato 2222\n");
                                fichero2 = fopen("doses.txt","w");
                                fprintf(fichero2,"%d",data6);
                                break;

                                case 3333:
                                printf("se guardo el dato 3333\n");
                                break;

                                case 4444:
                                printf("se guardo el dato 4444\n");
                                break;
                            }
                            //escribir_txt();
                        }
                    }else{
                        printf("se consumio SC5\n");
                        switch(data5){
                            case 1111:
                            printf("se guardo el dato 1111\n");
                            break;

                            case 2222:
                            printf("se guardo el dato 2222\n");
                            break;

                            case 3333:
                            printf("se guardo el dato 3333\n");
                            break;

                            case 4444:
                            printf("se guardo el dato 4444\n");
                            break;
                        }
                    }
                }else{
                    printf("se consumio SC4\n");
                    switch(data4){
                        case 1111:
                        printf("se guardo el dato 1111\n");
                        break;

                        case 2222:
                        printf("se guardo el dato 2222\n");
                        break;

                        case 3333:
                        printf("se guardo el dato 3333\n");
                        break;

                        case 4444:
                        printf("se guardo el dato 4444\n");
                        break;
                        }
                }
            }else{
                printf("se consumio SC3\n");
                switch(data3){
                    case 1111:
                    printf("se guardo el dato 1111\n");
                    break;

                    case 2222:
                    printf("se guardo el dato 2222\n");
                    break;

                    case 3333:
                    printf("se guardo el dato 3333\n");
                    break;

                    case 4444:
                    printf("se guardo el dato 4444\n");
                    break;                    
                }
            }
        }else{
            printf("se consumio SC2\n");
            switch(data2){
                case 1111:
                printf("se guardo el dato 1111\n");
                break;

                case 2222:
                printf("se guardo el dato 2222\n");
                break;

                case 3333:
                printf("se guardo el dato 3333\n");
                break;

                case 4444:
                printf("se guardo el dato 4444\n");
                break;                    
            }
        }
    }else{
        printf("se consumio SC1\n");
        switch(data1){
            case 1111:
            printf("se guardo el dato 1111\n");
            break;

            case 2222:
            printf("se guardo el dato 2222\n");
            break;

            case 3333:
            printf("se guardo el dato 3333\n");
            break;

            case 4444:
            printf("se guardo el dato 4444\n");
            break;                    
        }
    }   
}

void escribir_txt(){

}
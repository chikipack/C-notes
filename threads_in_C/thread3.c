#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* myTurn(void* arg){

    int *ptr = (int*)malloc(sizeof(int));
    *ptr = 100;
    for(int i=0;i<8; i++){
        sleep(1);
        printf("my turn! %d ||%d\n", i,*ptr);
        (*ptr)++;
    }
    return ptr;
}

void  yourTurn(){
    for (int j=0;j<3;j++){
        sleep(2);
        printf("your turn! %d \n",j);
    }
}

struct test{
    int age;
    char initial;
};


int main(){
    pthread_t th1;
    int *result;
    pthread_create(&th1, NULL, myTurn,NULL);
    yourTurn();
    pthread_join(th1,(void*)&result);
    printf("Thread is done, *result=%d\n",*result);

    return 0;
}
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* myTurn(void* arg){
    float *ptr = (float*)arg;
    for(int i=0;i<8; i++){
        sleep(1);
        printf("my turn! %d ||%f\n", i,*ptr);
        (*ptr)++;
    }
    return NULL;
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
    //heew we're gonna create a variable to pass to the thread
    int v = 100;
    pthread_create(&th1, NULL, myTurn,&v);
    yourTurn();
    pthread_join(th1, NULL);
    printf("Thread is done, the actual value of v is: %f\n",v);

    return 0;
}
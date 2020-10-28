#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* myTurn(void* arg){
    //here we create a integer pointer because we're reciving a integer type
    //from the main function
    //then, arg is a void pointer so we need to cast into a integer pointer

    //Conclusion: if we recive a variable (int, char, etc) from the main function
    //and senden by the last parameter un pthread_create function, 
    //that variabel we need to recived in a pointer of that variable 
    //example:
    // we send a integer from main
    // in the funcition we recive that variable as a integer pointer
    // main -> int
    // the_function -> int *
    //and then the arg must be casted from void* (void pointer) to type* (any type pointer)
    int *ptr = (int*)arg;
    for(int i=0;i<8; i++){
        sleep(1);
        printf("my turn! %d ||%c\n", i,*ptr);
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
    printf("Thread is done, the actual value of v is: %i\n",v);

    return 0;
}
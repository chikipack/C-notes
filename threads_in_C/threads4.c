#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//trhead safety

#define BIG 1000000000UL
uint32_t counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void * count_to_big(void * arg){
    for(uint32_t i=0; i<BIG; i++){
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock); 
    }
}

int main(){
    pthread_t thread;
    pthread_create(&thread, NULL, count_to_big, NULL);
    count_to_big(NULL);
    pthread_join(thread, NULL);
    printf("Done. Counter = %u\n",counter);
}
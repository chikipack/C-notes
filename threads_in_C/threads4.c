#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>//check
#include <string.h>
#include <stdint.h>
//trhead safety

//standard HTTP port
#define SERVER_PORT 80

#define MAXLINE 4096
#define SA struct sockaddr


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

char *getHomePage(char *ipaddr, int *bytesread){
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char *buffer = malloc(MAXLINE);

    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        return NULL;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, ipaddr, &servaddr.sin_addr)<=0){
        return NULL;
    }

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr))<0)
    {
        return NULL;
    }
    
    
}

int main(){
    pthread_t thread;
    pthread_create(&thread, NULL, count_to_big, NULL);
    count_to_big(NULL);
    pthread_join(thread, NULL);
    printf("Done. Counter = %u\n",counter);
}
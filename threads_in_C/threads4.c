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

    //we're connected, Prepare the message
    sprintf(sendline,"GET / HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);

    //send the request
    if (write(sockfd, sendline, sendbytes) != sendbytes){
        return NULL;
    }
    memset(buffer, 0, MAXLINE);

    //now read the first MAXLINE server's response
    n = read(sockfd, buffer, MAXLINE-1);
    if (n<0){
        return NULL;
    }
    *bytesread = n;
    return buffer;
}

int main(){
    pthread_t thread;
    char *buf;
    int buflen;

    pthread_create(&thread, NULL, count_to_big, NULL);
    //count_to_big(NULL);
    buf = getHomePage("172.217.0.78", &buflen);
    pthread_join(thread, NULL);
    printf("Done. Counter = %u. Recved %d bytes\n",counter, buflen);
}
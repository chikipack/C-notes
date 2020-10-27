#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* myTurn(void* arg){
    for(int i=0;i<8; i++){
        sleep(1);
        printf("my turn! %d\n", i);
    }
    return NULL;
}

void  yourTurn(){
    for (int j=0;j<3;j++){
        sleep(2);
        printf("your turn! %d \n",j);
    }
}

/*if you put attention to this code, you may know that dehe function myTurn is 
 *"bigger" than yourTurn function , so, yourTurn is gonna end first an kill the 
 * main process so, we need a way to prolongue the dead of main process until 
 * myTurn function ends, pthread_join() function allow us to do this
*/

int main(){
    pthread_t th1;
    pthread_create(&th1, NULL, myTurn,NULL);
    /* if we put the pthread_join function before the call to yourTurn Function
    this is gonna be the output
        my turn! 0
        my turn! 1
        my turn! 2
        my turn! 3
        my turn! 4
        my turn! 5
        my turn! 6
        my turn! 7
        your turn! 0 
        your turn! 1 
        your turn! 2 
    */
    //pthread_join(th1, NULL);
    yourTurn();
    pthread_join(th1, NULL);
    return 0;
}
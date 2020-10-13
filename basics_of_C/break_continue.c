#include <stdio.h>

int main(){
    int num = 0;

    while (num <=8){
        num++;
        if (num == 2){
            continue;
        }//end of if
        printf("%d\n",num);
       // num ++; //si dejamos el num++ aqui el programa se cicla 
    }//end of while
    
}//end of main
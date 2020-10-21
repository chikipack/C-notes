#include <stdio.h>
long Factorial(long numero){
    if (numero <=1){
        return 1;
    }else{
        return(numero * Factorial(numero-1));
    }
}

int main(){
    int numero;
    printf("ingresa un numero\n");
    scanf("%d",&numero);
    printf("\n");
    for (int i = 0; i <= numero; i++){
        printf("%ld\n",Factorial(i));
    }
    
    return 0;
}


#include<stdio.h>

int main(){
    int casos;
    printf("ingresa un numero\n");
    scanf("%i",&casos);
    switch(casos){
        case 1:
        printf("elegiste el caso 1\n");
        break;

        case 2:
        printf("elegiste el caso 2\n");
        break;
        
        default:
        printf("no encontramos tu caso\n");
        break;
    }
    return 0;
}
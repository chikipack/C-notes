#include <stdio.h>

int suma();
int main(){
    int num1, num2;
    printf("ingresa el primer valor:\n");
    scanf("%d",&num1);
    printf("ingresa el segundo valor:\n");
    scanf("%d",&num2);
    printf("%i\n",suma(num1, num2));
    return 0;
}

int suma(int num, int num2){
    int suma = num + num2;
    return suma;
}
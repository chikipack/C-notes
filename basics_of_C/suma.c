#include <stdio.h>
int main (){
    int a1, a2;
    float res;
    int a=12;
    int b=3;
    printf("ingrese el primer numero\n");
    scanf("%d",&a1);
    printf("ingrese el segundo numero\n");
    scanf("%d",&a2);
    res = (float)a1/(float)a2;
    printf("el resultado es: %f\n", res);
    return 0;   
}
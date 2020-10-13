#include <stdio.h>

int main(){
    //array unidimencional
    // int array1[4]={0,1,2,3};
    // printf("%i\n", array1[2]);
   
    //array semi-dinamico
    // int array_size;
    // printf("size of array\n");
    // scanf("%d",&array_size);
    // int age[array_size];
    // int i=0;
    // while (i < array_size){
    //     printf("ingresa la edad %d\n",i+1);
    //     scanf("%d",&age[i]);
    //     i++;
    // }
    // printf("los valores del arreglo son:\n");
    // for (int i = 0; i < array_size; i++){
    //     printf("%d\n", age[i]);
    // }
    //      [filas=horizontales][columnas=verticales]
    int array_multi[2][3]={{5,3,1},{6,2,0}};
    //         =============       
    //col      | 0 | 1 | 2 |
    //         =============
    //fila0    | 5 | 3 | 1 |
    //fila1    | 6 | 2 | 0 |
    //
    printf("%d\n", array_multi[1][1]);
    
    return 0;
}
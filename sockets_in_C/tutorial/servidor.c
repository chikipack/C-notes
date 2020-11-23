#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]){
    if(argc<2){
        //especifica los argumentos
        printf("%s <puerto>\n");argv[0];
        return -1;
    }

    int conexion_servidor, conexion_cliente, puerto;
    socklen_t longc; //declaramos una variable que contendra la
    //logitud de la estructura

    struct sockaddr_in servidor, cliente;
    char buffer[100];

    puerto = atoi(argv[1]);
    conexion_servidor = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&servidor, sizeof(servidor));
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = INADDR_ANY;

    if(bind(conexion_servidor,(struct sockaddr *)&servidor,sizeof(servidor))<0){
        printf("error al asociar el puerto a la conexion\n");
        close(conexion_servidor);
        return -1;
    } 
    listen(conexion_servidor,3);
    printf("a la escucha en el puerto %d\n",ntohs(servidor.sin_port));
    longc = sizeof(cliente);
    conexion_cliente = accept(conexion_servidor, (struct sockaddr *)&cliente,&longc);

    if(conexion_cliente <0){
        printf("error al aceptar trafico\n");
        close(conexion_servidor);
        return -1;
    }
    printf("conectando con %s:%d\n",inet_ntoa(cliente.sin_addr),htons(cliente.sin_port));
    if(recv(conexion_cliente, buffer, 100, 0) < 0){
        printf("error al recibir datos\n");
        close(conexion_servidor);
        return -1;
    }else{
        printf("%s\n",buffer);
        bzero((char*)&buffer,sizeof(buffer));
        send(conexion_cliente, "Recibido\n",13,0);
    }
    close(conexion_servidor);
    return 0;
}
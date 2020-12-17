#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <pthread.h>
#define MAX 3500
//
void * function();
FILE *new_file;

int main(int argc, char **argv){
	if(argc<2){ 					//validacion de entrada
		printf("%s <puerto>\n",argv[0]);//
		return 1;					//
	}

	/////////////////////////////////////////////////////////////
	//CONFIGURACION: aqui empieza la configuracion del servidor//
	/////////////////////////////////////////////////////////////

	int socket_server; 				//socket_server es el File Descriptor(fd) que regresa socket
	int conexion_cliente; 			//conexion_cliente es el canal de comunicacion con el cliente
	int data_recv;					//entero para guardar la cantidad de datos recibidos por recv()
	int puerto; 					//puerto es la variable donde vamos a guardar el puerto a usar
	puerto = atoi(argv[1]);			//convertimos el argumento recibido desde consola a un int para poder usarlo
	
	socklen_t longc; 				//Debemos declarar una variable que contendrá la longitud de la estructura
	struct sockaddr_in servidor; 	//estructura con la informacion del servidor
	struct sockaddr_in cliente;		//estructura con la informacion del cliente
	/*
			ASI ES COMO SE VE LA ESTRUCTURA sockaddr_in
			la cual debe ser llenada
		#include <netinet/in.h>

		struct sockaddr_in {
			short            sin_family;   // e.g. AF_INET
			unsigned short   sin_port;     // e.g. htons(3490)
			struct in_addr   sin_addr;     // see struct in_addr, below
			char             sin_zero[8];  // zero this if you want to
		};

		struct in_addr {
			unsigned long s_addr;  // load with inet_aton()
		};
	*/

	//bzero((char *)&servidor, sizeof(servidor)); 									//llenamos la estructura de 0's para que este limpia
	servidor.sin_family = AF_INET; 													//asignamos a la estructura la familia 
	servidor.sin_port = htons(puerto);												//asignamos a la estructura el puerto
	servidor.sin_addr.s_addr = INADDR_ANY; 											//esta macro especifica nuestra dirección
	bzero(&(servidor.sin_zero),8);

	char buffer[MAX]; 				//Declaramos una variable que contendrá los mensajes que recibamos
	
	if((socket_server = socket(AF_INET, SOCK_STREAM, 0))<0){						//creamos el socket
		printf("socket error\n");
		return -1;
	}

	if(bind(socket_server, (struct sockaddr *)&servidor, sizeof(servidor)) < 0){	//Con bind hacemos publico el servicio
		printf("Error al asociar el puerto a la conexion\n");
		close(socket_server);
		return 1;
	}
	
	if((listen(socket_server, 3))<0){ 												//Estamos a la escucha
		printf("listen error\n");
		return -1;	
	}

	printf("A la escucha en el puerto %d\n", ntohs(servidor.sin_port));
	longc = sizeof(cliente); 														//Asignamos el tamaño de la estructura cliente a esta variable
	
	

	while(1){
		int *id_child = (int*)malloc(sizeof(int));
		*id_child = accept(socket_server,(struct sockaddr*)&cliente,&longc);

		if((*id_child)<0){
			printf("accept error\n");
			return -1;
		}else{
			printf("server accept the client\n");
		}

		pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t));

		pthread_create(thread, NULL, function, (void*)id_child);

		free(thread);
		
		
	}
	close(socket_server);

	return 0;
}

void * function(void * arg){
	int n;
	char buffer[MAX];
	char nbuffer[]="dale papi, manda el paquetaxo\n";
	char aux[40];
	char new_file_name[]="copy_of_";

	int *id_ch = (int*)arg;
	
	
	int nbytes_recv;
	int nbytes_recv2;
	bzero(buffer, MAX);
	bzero(aux,30);
	nbytes_recv2 = recv(*id_ch, aux, sizeof(aux), 0);
	printf("el archivo enviado desde el cliente tiene por nombre: %s\n",aux);
	strcat(new_file_name,aux);
	printf("la copia se guardara en: %s\n",new_file_name);
	new_file = fopen(new_file_name,"wb");
	if (new_file == NULL){
		perror("error al abrir/crear el archivo\n");
		return NULL;
	}
	send(*id_ch, nbuffer, strlen(nbuffer), 0);
	while(nbytes_recv != 0){
		nbytes_recv = recv(*id_ch, buffer, sizeof(buffer), 0);
		printf("recibi %d bytes\n",nbytes_recv);
		if(nbytes_recv == 0){
			printf("se recibio el arvhivo completamente\n");
			bzero(buffer, MAX);
			break;
		}
		fwrite(buffer,1, nbytes_recv, new_file);
		//send(*id_ch,nbuffer, strlen(nbuffer),0);
	}
	bzero(buffer, MAX);
	fclose(new_file);
	// do{
	// 	bzero(buffer, MAX);
	// 	int nbytes_recv = recvfrom(*id_ch, buffer, sizeof(buffer),0,NULL, NULL);

	// 	printf("mensaje recibido: %s",buffer);
	// 	if(strncmp("exit", buffer, 4)==0){
	// 		printf("el cliente te mando alv\n");
	// 		break;
	// 	}
	// 	bzero(buffer, MAX);
	// 	send(*id_ch, nbuffer, strlen(nbuffer), 0);
	// }while(strncmp("exit", buffer, 4));

	close(*id_ch);
	free(id_ch);

}
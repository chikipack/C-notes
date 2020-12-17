#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
//
#define h_addr  h_addr_list[0]
#define MAX 3500

FILE *original_file;
char file_name[];
int main(int argc, char **argv){
	
	int in;

	if(argc<2){                         //validacion para usar correctamente el programa
		printf("<host> <puerto>\n");
		return 1;
	}

	int puerto, conexion;
	char buffer[MAX];

	puerto=(atoi(argv[2])); 			//conversion del argumento
	
	struct sockaddr_in cliente; 	    //Declaración de la estructura con información para la conexión
	struct hostent *servidor; 	        //Declaración de la estructura con información del host

	/*
	#include <netdb.h>

		struct  hostent {
			char *  h_name;     
			char ** h_aliases; 
			int     h_addrtype;  
			int     h_length;    
			char ** h_addr_list;
		};

	#define h_addr  h_addr_list[0]
	*/

	servidor = gethostbyname(argv[1]); //Asignacion

	/*
		The gethostbyname() function returns a structure of type hostent for
       	the given host name.  Here name is either a hostname or an IPv4
       	address in standard dot notation (as for inet_addr(3)).  If name is
       	an IPv4 address, no lookup is performed and gethostbyname() simply
       	copies name into the h_name field and its struct in_addr equivalent
       	into the h_addr_list[0] field of the returned hostent structure.
	*/

	if(servidor == NULL){ 										//Comprobación
		printf("Host erróneo\n");
		return 1;
	}
	
	

	if((conexion = socket(AF_INET, SOCK_STREAM, 0))<0){			//Asignación del socket
		printf("socket error\n");
		return -1;
	}	
						
	bzero((char *)&cliente, sizeof((char *)&cliente)); 			//Rellena toda la estructura de 0's
	cliente.sin_family = AF_INET; 								//asignacion del protocolo
	cliente.sin_port = htons(puerto); 							//asignacion del puerto
	cliente.sin_addr = *((struct in_addr*)servidor->h_addr);	//asiganacion de la direccion
	
	if((connect(conexion,(struct sockaddr *)&cliente, sizeof(cliente))) < 0){ 
		//conectando con el host
		printf("Error conectando con el host\n");
		close(conexion);
		return 1;
	}
	int leidos;
	char aux[40];
	printf("introduce el nombre del arvhivo:\n");
	scanf("%s",&file_name);
	send(conexion, file_name, 40, 0);
	in = recv(conexion, aux, sizeof(aux), 0);
	printf("recived from server: %s",aux);
	original_file = fopen(file_name,"rb");
	if (original_file == NULL){
		printf("error abriendo el archivo\n");
		return -1;
	}
	
	leidos =1;
	//meter el read antes del send
	while(leidos != 0){
		leidos = fread(buffer, 1, MAX, original_file);
		in = send(conexion, buffer, 3500, 0);
		printf("Hola \n");
		//leidos = fread(buffer, 1, MAX, original_file);
		// //send message to server
		// printf("escribe un mensaje:");
		// bzero(buffer, 100);
		// fgets(buffer,100,stdin);
		// printf("sending to server: %s ",buffer);
		// in = send(conexion, buffer, 100,0);
		// if(in<0){
		// 	perror("Client error: writing to server");
		// 	return 0;
		// }
		// if (strncmp("exit",buffer,4)==0){
		// 	printf("El cliente cerro la conexion, notificando al servidor...");
		// 	break;
		// }
		
		// printf("esperando respuesta del servidor...\n");
		// //now recive the response
		// bzero(buffer,100);
		// in = recv(conexion, buffer, 100, 0);
		// if (in < 0) {
		// 	perror("\nClient Error: Reading from Server");
		// 	return 0;
		// }
		// printf("\nReceived FROM SERVER: %s ",buffer);
		// if (strncmp("exit", buffer,4)==0){
		// 	printf("el servidor envio exit...terminando conexion");
		// 	break;
		// }
		
	}
	fclose(original_file);
	close(conexion);
return 0;
}

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

int main(int argc, char **argv){
	
	int in;

	if(argc<2){                         //validacion para usar correctamente el programa
		printf("<host> <puerto>\n");
		return 1;
	}

	int puerto, conexion;
	char buffer[100];

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
	
	//bcopy((char *)servidor->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(servidor->h_length));
	//bcopy(); copia los datos del primer elemento en el segundo con el tamaño máximo del tercer argumento.
	/*
	con esto lo que estamos haciendo es copiar la direccion IPv4 guardada en h_addr(que esta en la estrucura hostent) 
	(que es la direccion IP obtenida que introducimos como argumento) en la estructura cliente, que si bien es la estructura 
	que se le va a asocicar al cliente, esta tiene que contener la informacion del servidor al que se va a conectar
	para que cuando se llame a la funcion connect(), nuestro cliente se pueda conectar con el servidor
	*/

	//cliente.sin_addr = *((struct in_addr *)servidor->h_addr); //<--para empezar prefiero que se usen
	//inet_aton(argv[1],&cliente.sin_addr); //<--alguna de estas dos funciones
	
	if((connect(conexion,(struct sockaddr *)&cliente, sizeof(cliente))) < 0){ 
		//conectando con el host
		printf("Error conectando con el host\n");
		close(conexion);
		return 1;
	}
	//printf("Conectado con %s:%d\n",inet_ntoa(cliente.sin_addr),htons(cliente.sin_port));
	while(1){
		//send message to server
		printf("escribe un mensaje:");
		bzero(buffer, 100);
		fgets(buffer,100,stdin);
		printf("sending to server: %s ",buffer);
		in = send(conexion, buffer, 100,0);
		if(in<0){
			perror("Client error: writing to server");
			return 0;
		}
		if (strncmp("exit",buffer,4)==0){
			printf("El cliente cerro la conexion, notificando al servidor...");
			break;
		}
		
		printf("esperando respuesta del servidor...\n");
		//now recive the response
		bzero(buffer,100);
		in = recv(conexion, buffer, 100, 0);
		if (in < 0) {
			perror("\nClient Error: Reading from Server");
			return 0;
		}
		printf("\nReceived FROM SERVER: %s ",buffer);
		if (strncmp("exit", buffer,4)==0){
			printf("el servidor envio exit...terminando conexion");
			break;
		}
		
	}
	close(conexion);
return 0;
}
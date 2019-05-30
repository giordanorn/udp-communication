#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

int
main(int argc, char *argv[])
{
 
	const char* PROGRAM = argv[0];
	const char* DESTINATION_HOST = argv[1];

	if ( argc < 3 )
	{
		printf("Usage: %s <message1> <message2> ... <messageN>\n", PROGRAM);
		exit(1);
	}

	
	// OBTEM O ENDERECO IP e PESQUISA O NOME NO DNS
	struct hostent *host;
	host = gethostbyname(DESTINATION_HOST);
	if ( host == NULL )
	{
		printf("%s: Hostname error! Unknown host %s.\n", PROGRAM, DESTINATION_HOST);
		exit(1);
	}

	const char *HOST_NAME = host->h_name;
	const char *HOST_IP_ADDRESS = inet_ntoa(*(struct in_addr *)host->h_addr_list[0]);
	printf("%s: Trying to send data to %s (IP: %s)...\n", PROGRAM, HOST_NAME, HOST_IP_ADDRESS);

	struct sockaddr_in client_address;
	struct sockaddr_in remote_server_addr;
	
	// CONFIGURANDO ESTRUTURA REFERENTE AO HOST REMOTO (SERVIDOR)
	remote_server_addr.sin_family = host->h_addrtype;
	memcpy((char *) &remote_server_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	remote_server_addr.sin_port = htons(REMOTE_SERVER_PORT);

	// 1. CRIA O SOCKET
	int sd, rc;
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if ( sd < 0 )
	{
		printf("%s: Socket error! Could not open socket.\n", PROGRAM);
		exit(1);
	}

	// 2. FAZ A LIGACAO (BIND) DE TODOS OS ENDERECOS COM A PORTA
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = htonl(INADDR_ANY);
	client_address.sin_port = htons(0);
	rc = bind(sd, (struct sockaddr *) &client_address, sizeof(client_address));
	if ( rc < 0 )
	{
		printf("%s: Socket error! Could not bind port.\n", PROGRAM);
		exit(1);
	}

	// 3. ENVIA DADOS
	for ( int i = 2; i < argc; i++)
	{
		rc = sendto(sd, argv[i], strlen(argv[i])+1, 0, (struct sockaddr *) &remote_server_addr, sizeof(remote_server_addr));
		if ( rc < 0 )
		{
			printf("%s: Could not send message: %s\n", PROGRAM, argv[i-1]);
			close(sd);
			exit(1);
		}
	}

	return 0;
}
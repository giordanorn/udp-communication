#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100

int
main(int argc, char *argv[])
{
	int sd, rc, n; socklen_t cliLen;
	struct sockaddr_in cliAddr, servAddr;

	const char* PROGRAM = argv[0];

	/* 1. CRIA O SOCKET */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if ( sd < 0 )
	{
		printf("%s: Socket error! Could not open socket.\n", PROGRAM);
		exit(1);
	}
	
	/* 2. FAZ A LIGACAO (BIND) DE TODOS OS ENDERECOS COM A PORTA */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	rc = bind(sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
	if ( rc < 0 )
	{
		printf("%s: Could not connect to port %d.\n", PROGRAM, LOCAL_SERVER_PORT);
		exit(1);
	}

	printf("%s: Waiting for data on port UDP(%d)\n", PROGRAM, LOCAL_SERVER_PORT);
	
	char msg[MAX_MSG];
	/* 3. LOOP INFINITO PARA LEITURA (LISTEN) */
	while (true)
	{
		/* INICIA O BUFFER DE COMUNICACAO */
		memset(msg, 0x0, MAX_MSG);
		
		/* RECEBE MENSAGEM */
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &cliAddr, &cliLen);
		if ( n < 0 )
		{
			printf("%s: Could not receive data.\n", PROGRAM);
			continue;
		}

		/* IMPRIME A MENSAGEM RECEBIDA */
		printf("%s: Data received from %s UDP(%d): %s\n", PROGRAM, inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), msg);
	}
	return 0;
}

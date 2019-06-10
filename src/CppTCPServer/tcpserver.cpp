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
#include <string>
#include <iostream>

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100
using namespace std;

int
main(int argc, char *argv[])
{
	int socket_server, rc, n; socklen_t cliLen;
	struct sockaddr_in cliAddr, servAddr;

	const char* PROGRAM = argv[0];

	/* 1. CRIA O SOCKET */
	socket_server = socket(AF_INET, SOCK_DGRAM, 0);
	if ( socket_server < 0 )
	{
		printf("%s: Socket error! Could not open socket.\n", PROGRAM);
		exit(1);
	}
	
	/* 2. FAZ A LIGACAO (BIND) DE TODOS OS ENDERECOS COM A PORTA */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	rc = bind(socket_server, (struct sockaddr *) &servAddr,sizeof(servAddr));
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
		n = recvfrom(socket_server, msg, MAX_MSG, 0, (struct sockaddr *) &cliAddr, &cliLen);
		if ( n < 0 )
		{
			printf("%s: Could not receive data.\n", PROGRAM);
			continue;
		}

		std::string result = "Mensagem ";
		result = result.append(msg) + " recebida às 99h99 (hora local)\n";
		cout << result;

		sendto(socket_server, result.c_str(), result.length(), 0, (struct sockaddr *) &cliAddr, cliLen);

		/* IMPRIME A MENSAGEM RECEBIDA */
		//printf("%s: Data received from %s UDP(%d): %s\n", PROGRAM, inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), msg);
	}
	return 0;
}

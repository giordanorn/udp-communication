#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/socket.h> 
#include <pthread.h>

#define MAX 80 
#define PORT 8080 

void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	do {
		// clear buffer
		bzero(buff, sizeof(buff)); 

		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n'); 

		send(sockfd, buff, sizeof(buff), 0); 

		// clear buffer
		bzero(buff, sizeof(buff)); 

		// blocks until server send message
		recv(sockfd, buff, sizeof(buff), 0); 

		printf("From Server : %s\n", buff); 

	} while ( (strncmp(buff, "exit", 4)) == 0 );
	printf("Client Exit...\n"); 

} 

void
listen_server(int sockfd)
{
	char buff[MAX]; 
	do {
		// clear buffer
		bzero(buff, sizeof(buff)); 

		// blocks until server send message
		recv(sockfd, buff, sizeof(buff), 0); 

		printf("From Server : %s\n", buff); 
	} while ( (strncmp(buff, "exit", 4)) == 0 );
}

void
create_listen_thread(int sockfd)
{
	//pthread_t thread;
	//int rc;	

	//rc = pthread_create(&thread, NULL, listen_server, (void *)&sockfd);
}

void *
send_message(void *sockfd1)
{
	int *sockfd = (int *) sockfd1;
	printf("opa %d\n", *sockfd);
	char buff[MAX]; 
	int n;
	do {
		// clear buffer
		bzero(buff, sizeof(buff)); 

		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n'); 

		send(*sockfd, buff, sizeof(buff), 0); 

	} while ( (strncmp(buff, "exit", 4)) == 0 );
}

int
create_send_thread(int *sockfd)
{
	printf("Criando a send thread. sockfd=%d\n", *sockfd);
	pthread_t thread;
	return pthread_create(&thread, NULL, send_message, (void *)&sockfd);
}

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
	{
		printf("Socket successfully created..\n"); 
	}
	
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 

	int *sockfdp = (int *) calloc(sizeof(int), 1);
	*sockfdp = sockfd;
	create_send_thread(sockfdp);
	listen_server(sockfd);
	//create_send_thread(sockfd);

	// close the socket 
	close(sockfd); 
} 


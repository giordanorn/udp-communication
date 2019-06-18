#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <stdlib.h>

#define MAX 80 

using namespace std;

int sockfd;

void func() 
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



void send_message()
{
	char buff[MAX]; 
	int n;
	do {
		// clear buffer
		bzero(buff, sizeof(buff)); 

		n = 0; 

		while ((buff[n++] = getchar()) != '\n'); 

		send(sockfd, buff, sizeof(buff), 0); 

		cout << "I've sent the message.\n";

	} while ( (strncmp(buff, "exit", 4)) != 0 );
}


int main(int argc, char** argv) 
{ 
	int PORT = atoi(argv[1]);
	struct sockaddr_in servaddr; 

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

	std::thread t1 = std::thread(send_message);

	char buff[MAX]; 
	do {
		// clear buffer
		bzero(buff, sizeof(buff)); 

		// blocks until server send message
		recv(sockfd, buff, sizeof(buff), 0); 

		printf("From Server : %s\n", buff); 
	} while ( (strncmp(buff, "exit", 4)) == 0 );
	cout << "SAÍ DO DO WHILE\n";

	// close the socket 
	close(sockfd); 
} 


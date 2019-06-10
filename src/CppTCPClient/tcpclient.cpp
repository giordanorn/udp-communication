// Client side C/C++ program to demonstrate Socket programming 
#include <iostream> 
#include <cstdio> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <cstring> 
#define PORT 8080 


void receive_from_server(int);

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	std::string str = "Hello from client";
	const char *hello = str.c_str(); 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 


	write(sock , hello, strlen(hello)); 
	printf("Hello message sent\n"); 

	receive_from_server(sock);

	return 0; 
} 

void
receive_from_server(int sock)
{
	char buffer[1024] = {0}; 
	while(1)
	{
		int valread;
		valread = recv(sock , buffer, 1024, 0); 
		printf("%s\n", buffer); 
	}
}



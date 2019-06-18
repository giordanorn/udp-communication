#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <stdlib.h>
#include "Server.h"
#include "ClientConn.h"


void listenToClient(int sock){
	cout << "Starting to listenToClient" << " " << sock << endl;
	char buffer[1024] = {0};

    while(true){


        int val_read = read( sock , buffer, 1024);

        if(val_read < 0){
            printf("recv error: %s\n", strerror(val_read));
            break;
        }

        else{
        	printf("%s\n", buffer);
        }
    }
    cout << "Stopped to listen to the client's messages.\n";
}

// Driver function 
void createSocket(sockaddr_in serverAddress, int port) 
{ 
	cout << "Creating a socket\n";
	struct sockaddr_in cli; 
	unsigned int len; 
	int connfd;

	// socket create and verification 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&serverAddress, sizeof(serverAddress)); 

	// assign IP, port 
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
	serverAddress.sin_port = htons(port); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (sockaddr*) & serverAddress, sizeof(serverAddress))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 1)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 

	len = sizeof(cli); 

	// Server* server = new Server();

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
	{
		printf("server acccept the client...\n"); 	

		// ClientConn* conn = new ClientConn(server, serverAddress, sockfd);
		// conn->startThread();

		// code for printing the client's address
		{
			char str[INET_ADDRSTRLEN];
			inet_ntop( AF_INET, &cli.sin_addr, str, INET_ADDRSTRLEN );
			string addr = string(str);
			cout << "client's address is " << addr << endl;

			// server->add_client(addr, conn);
		}

		// listenToClient(sockfd);
		cout << "Starting to listenToClient" << " " << connfd << endl;
		char buffer[1024] = {0};

	    while(true){


	        int val_read = read( connfd , buffer, 1024);

	        if(val_read < 0){
	            printf("recv error: %s\n", strerror(val_read));
	            break;
	        }

	        else{
	        	printf("%s\n", buffer);
	        }
	    }
	    cout << "Stopped to listen to the client's messages.\n";
		}

	// After chatting close the socket 
	close(sockfd); 
	cout << "Fechou\n";
} 

int main(int argc, char** argv){
    int port1 = 6666;
    int port2 = 7777;
    struct sockaddr_in servaddr;

    // it blocks on the "connect" method
    std::thread t1 = std::thread(createSocket, servaddr, port1);
    std::thread t2 = std::thread(createSocket, servaddr, port2);


    cout << "Segurando os role pra que a  main nao para ieieieieieieie\n";

    while(true);

    return 0;
}
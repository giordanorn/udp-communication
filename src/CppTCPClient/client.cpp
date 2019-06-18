#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define MSG_MAX_SIZE 80
#define SERVER_ADDR "127.0.0.1"

void listenToServer(){

}

void sendMessages(int sock){
    char buff[MSG_MAX_SIZE];
    int n;

    do {
        // clear buffer
        bzero(buff, sizeof(buff)); 

        n = 0; 

        while ((buff[n++] = getchar()) != '\n'); 

        // Removes the \n from the message that the client sent
        buff[n-1] = '\0';

        send(sock, buff, sizeof(buff), 0); 
    } while ( (strncmp(buff, "exit", 4)) != 0 );
}

int main(int argc, char** argv){

    int serverPort = atoi(argv[1]);
    // Result code of socket operations
    int resultCode;

    // Creates a new socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server's address
    struct sockaddr_in serverAddress; 
    bzero(&serverAddress, sizeof(serverAddress));  // Idk what's that's for
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDR); 
    serverAddress.sin_port = htons(serverPort); 

    // Connects the client socket to server socket 
    if ((resultCode = connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) { 
        cout << "ERROR: connection with the server failed." << strerror(resultCode) << endl;   
        exit(0); 
    } else{
        cout << "Connected to the server." << endl; 
    }

    // Listens to the server messages in a separate thread
    thread threadListenToServer = thread(listenToServer);

    // Sends messages to the server
    sendMessages(sock);

    return 0;
}
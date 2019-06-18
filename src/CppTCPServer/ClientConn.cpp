#include "ClientConn.h"
#include <thread>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> // read() 
#include <stdlib.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h> 

#define inet_addrstrlen 1024

ClientConn::ClientConn(Server* server, struct sockaddr_in  address, int sock){
    this->server = server;
    this->address = address;
    this->sock = sock;
    
}

void ClientConn::startThread(){
    this->t1 = std::thread(&ClientConn::listen_to_messages, this);
}

void ClientConn::send_message(char *message){
    send(this->sock, message, strlen(message), 0);
}
void ClientConn::listen_to_messages(){
    cout << "Started to listen to the client's messages.\n";
    char buffer[1024] = {0};

    while(true){


        int val_read = read( this->sock , buffer, 1024);


        if(val_read < 0){
            printf("recv error: %s\n", strerror(val_read));
            break;
        }
        char str[inet_addrstrlen];
        inet_ntop(AF_INET, &(this->address.sin_addr), str, inet_addrstrlen); 
        this->server->send_to_almost_all(buffer, str);
    }
    cout << "Stopped to listen to the client's messages.\n";
}
void ClientConn::exit(){

}

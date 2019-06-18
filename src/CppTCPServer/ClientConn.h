#ifndef ClientConn_H
#define ClientConn_H

class Server;

#include <string>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <thread>
#include "Server.h"

using namespace std;

class ClientConn
{
private:
    struct sockaddr_in  address;
    string name;
    Server* server;
    int sock;
    std::thread t1;

public:
    ClientConn(Server* server, struct sockaddr_in  address, int sock);
    void send_message(char* message);
    void listen_to_messages();
    void exit();
    void startThread();


};




#endif 
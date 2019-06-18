#ifndef Server_H
#define Server_H

class ClientConn;

#include <unordered_map>
#include <iostream>
#include <string>
#include "ClientConn.h"
using namespace std;


class Server{
private:
    unordered_map<string, ClientConn*> clients;
public:
    Server();
    ~Server();
    void add_client(string address, ClientConn* clientConn);
    void remove_client(string addr);
    void send_to_almost_all(char* msg, string client_not_to_send);

};

#endif
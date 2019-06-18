#include "Server.h"

Server::Server(){

}
Server::~Server(){
    for (auto& x: clients) {
        delete x.second;
    }
}
void Server::add_client(string address, ClientConn* clientConn){
    clients[address] = clientConn;
}
void Server::remove_client(string addr){
    clients.erase(addr);
}
void Server::send_to_almost_all(char* msg, string client_not_to_send){
    for (auto& x: this->clients) {
        if(x.first == client_not_to_send)
            continue;
        x.second->send_message(msg);
    }
}
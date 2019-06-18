#include <iostream>
#include <thread>
#include <unordered_map>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define ADDRESS "127.0.0.1"
#define MAX_CLIENTS 50
#define CLIENT_MSG_SIZE 2000

struct Client{
    int port;
    string addr;

    bool operator==(const Client &other) const {return port == other.port && addr == other.addr;}
};

// Adapted from: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std {
    template <>
    struct hash<Client>
    {
        size_t operator()(const Client& client) const {return hash<string>()(client.addr) ^ (hash<int>()(client.port) << 1);}
    };
}

void listenToClient(int sock){
    char clientMessage[CLIENT_MSG_SIZE];

    while(true){
        recv(sock , clientMessage , CLIENT_MSG_SIZE , 0);

        cout << string(clientMessage) << endl;
    }
}

unordered_map<Client, thread> clientThreads;

// Adapted from: https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading
int main(int argc, char** argv){
    int port = atoi(argv[1]);

    // Result code from the socket's functions
    int resultCode;

    // Creates the server's socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Creates the server's address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ADDRESS);
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);

    // Binds the server's address to its socket
    if ((resultCode = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) == 0)   {
        cout << "Binding done.\n";
    }

    else{
        cout << "ERROR: can't binding on the socket. "  << strerror(resultCode) << endl;   
        exit(0);
    }

    // Makes the server listen on the socket
    if((resultCode = listen(serverSocket,MAX_CLIENTS))==0){
        cout << "Listening on the socket." << endl;
    }

    else{
        cout << "ERROR: can't listen on the socket. "  << strerror(resultCode) << endl;
        exit(0);
    }


    thread threadClient;

    while(true){

        // Waits for a new connection
        cout << "Waiting for a new connection...\n";
        struct sockaddr_in peerAddress;
        unsigned int sizePeerAddress = sizeof peerAddress;
        int socketToCommunicateWithClient = accept(serverSocket, (struct sockaddr*) &peerAddress, &sizePeerAddress);

        // Creates a new Client object.
        // TODO: put here the *actual* client's address and port.
        string a("o"); 
        Client client = Client();
        client.port = 1;
        client.addr = a;

        // Creates a thread for the new client
        threadClient = thread(listenToClient, socketToCommunicateWithClient);

        // Adds the thread in the map of threads
        clientThreads[client] = move(threadClient);

    }


    


    return 0;
}
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define MAX_CLIENTS 50
#define CLIENT_MSG_SIZE 2000

struct Client{
    u_short port;
    string addr;

    bool operator==(const Client &other) const {return port == other.port && addr == other.addr;}
};

// Adapted from: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std {
    template <>
    struct hash<Client>
    {
        size_t operator()(const Client& client) const {return hash<string>()(client.addr) ^ (hash<u_short>()(client.port) << 1);}
    };
}

void listenToClient(int sock, Client client){
    char clientMessage[CLIENT_MSG_SIZE];

    while(true){
        recv(sock , clientMessage , CLIENT_MSG_SIZE , 0);

        cout << string(clientMessage) << endl;
    }
}

void waitsForANewConnection(int serverSocket, unordered_map<Client, thread>& clientThreads){
    // Address of the incoming client
    struct sockaddr_in clientAddress;
    unsigned int sizeClientAddress = sizeof clientAddress;

    // Waits for a new connection
    cout << "Waiting for a new connection...\n";
    int socketToCommunicateWithClient = accept(serverSocket, (struct sockaddr*) &clientAddress, &sizeClientAddress);

    // Gets the client's address in IPv4 notation
    char ipAddressAsCharArray[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddress.sin_addr), ipAddressAsCharArray, INET_ADDRSTRLEN);
    string address = string(ipAddressAsCharArray);

    // Gets the client's port
    u_short port = clientAddress.sin_port;

    // Creates a new Client object. 
    Client client = {port, address};

    // Creates a thread for the new client
    thread threadClient;
    threadClient = thread(listenToClient, socketToCommunicateWithClient, client);

    // Adds the thread in the map of threads
    clientThreads[client] = move(threadClient);
}


// Adapted from: https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading
int
main (int argc, char** argv)
{
    unordered_map<Client, thread> clientThreads;
    unordered_set<string> usedNames;

	char *serverIPAddress = argv[1];
    int port = atoi(argv[2]);

    // Result code from the socket's functions
    int resultCode;

    // Creates the server's socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Creates the server's address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(serverIPAddress);
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);

    // Binds the server's address to its socket
    if ((resultCode = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) == 0)   {
        cout << "Binding done.\n";
    }

    else {
        cout << "ERROR: can't binding on the socket. "  << strerror(resultCode) << endl;   
        exit(0);
    }

    // Makes the server listen on the socket
    if ((resultCode = listen(serverSocket, MAX_CLIENTS))==0){
        cout << "Listening on the socket." << endl;
    }

    else {
        cout << "ERROR: can't listen on the socket. "  << strerror(resultCode) << endl;
        exit(0);
    }

    while (true)
    {
        waitsForANewConnection(serverSocket, clientThreads);
    }
    
    return 0;
}

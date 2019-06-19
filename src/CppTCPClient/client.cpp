#include <iostream>
#include <mutex>
#include <string>
#include <condition_variable>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // read()

using namespace std;

#define MSG_MAX_SIZE 80
#define QUIT_MESSAGE "sair"

int sock;
bool amIListening = false;
mutex mtx;
condition_variable cv;

void listenToServer(int sock){
    amIListening = true;
    char buff[MSG_MAX_SIZE];
    int res;
    string msg;

    while(true){
        res = read(sock, buff, MSG_MAX_SIZE);

        if(res < 0) break;

        msg = string(buff);

        if (msg == QUIT_MESSAGE)
            break;

        cout << msg << endl;
    }
}


void sendMessages(char* firstMsg){
    send(sock, firstMsg, (strlen(firstMsg)+1) * sizeof(char), 0); 
    string msg;
    
    do {
        getline(cin,msg);
        send(sock, msg.c_str(), msg.size(), 0); 
    } while ( QUIT_MESSAGE != msg);

    // cout << "parei de mandar tb\n";
}



int main(int argc, char** argv){
    if(argc != 4){
        cout << "You're missing/putting too many params.\n";
        exit(0);
    }

    int serverPort = atoi(argv[2]);
    // Result code of socket operations
    int resultCode;

    // Creates a new socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server's address
    struct sockaddr_in serverAddress; 
    bzero(&serverAddress, sizeof(serverAddress));  // Idk what's that's for
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]); 
    serverAddress.sin_port = htons(serverPort); 

    // Connects the client socket to server socket 
    if ((resultCode = connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) { 
        cout << "ERROR: connection with the server failed." << strerror(resultCode) << endl;   
        exit(0); 
    } else{
        cout << "Conectade ao servidor " << argv[1] << " na porta " << argv[2] <<  endl; 
    }

    // Listens to the server messages in a separate thread
    thread threadListenToServer = thread(listenToServer, sock);
    thread threadSendMessages = thread(sendMessages, argv[3]);

    threadListenToServer.join();
    threadSendMessages.join();    

    // unique_lock<mutex> lk(mtx);
    // cout << "to na baerrreira\n";

    // cv.wait(lk, []{return amIListening;});

    close(sock);

    return 0;
}
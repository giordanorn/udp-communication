## UDP Communication Server and Client
This repo stores the source files of two programs (`udpclient` and `udpserver`) which send and receive data through UDP protocol, both written in C++ (in the future, rewritten in C).

It is inspired on course Redes de Computadores given by Professor Silvio Costa Sampaio from UFRN.

This repo started as a copy of what the professor has given as a class activity.

## Clone and Compile
Make sure you have `git`, `g++` and `make` installed on your machine, then you may run the following commands on your terminal:
```sh
git clone https://github.com/giordanorn/udp-communication
cd udp-communication
make
```

## Usage
The two programs have diferent behaviours, you can see each one down below.
### udpserver
You need to run an instance of `udpserver` to start listening to messages. You can do it by typing:
```sh
./udpserver
```

After this, you might be able to receive messages.

### udpclient
This command sends message to a udpserver with its ip address.
```sh
./udpclient <IPADDRESS> <message1> <message2> ... <messageN>
```

### What it does
`udpserver` runs a instance of a udp server and wait for messages from a udp client.
`uspclient` send messages to an ip address which runas a udpserver through udp protocol.

### Requested features
Soon...

import socket                                         
import threading
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("port", help="the port in which the server will run",
                    type=int)
args = parser.parse_args()

class ClientConn:
    def __init__(self, server, conn, addr):
        self.server = server
        self.conn = conn
        self.addr = addr
        self.name = ""
        t = threading.Thread(target=self.listen_to_messages)
        t.start()

    def send_message(self, msg):
        self.conn.sendall(msg.encode('ascii'))

    def is_name_valid(self, name):
        return not (not name)

    def listen_to_messages(self):
        while True:
            msg = self.conn.recv(1024)
            decoded_msg = msg.decode('ascii')

            if not self.name:
                if not self.is_name_valid(decoded_msg):
                    self.send_message('please choose a valid name')

                else:
                    self.name = decoded_msg
                    self.server.send_to_almost_all(self.name + " entrou na sala", self.addr)

            elif decoded_msg == 'sair':
                self.server.send_to_almost_all(self.name + " saiu da sala", self.addr)                
                self.exit()

            else:
                self.server.send_to_almost_all(self.name + " disse: " + decoded_msg, self.addr)                

    def exit(self):
        self.conn.close()

class Server:
    def __init__(self, addr, port):
        self.addr = addr
        self.port = port
        self.clients = {}

    def add_client(self, addr, client):
        self.clients[addr] = client

    def send_to_almost_all(self, msg, client_not_to_send):
        for client_addr, client in self.clients.items():
            if client_addr == client_not_to_send:
                continue

            client.send_message(msg)


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
host = "127.0.0.1"    
port = args.port                              

server = Server(host, port)

sock.bind((host, port))                                  
sock.listen(5)                                           

while True:
    conn, addr = sock.accept()
    client = ClientConn(server, conn, addr[0])
    server.add_client(addr, client)
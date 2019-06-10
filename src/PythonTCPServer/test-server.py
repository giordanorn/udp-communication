import socket                                         
import threading
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("port", help="the port in which the server will run",
                    type=int)
args = parser.parse_args()

class ClientConn:
    def __init__(self, server, conn, addr, lock):
        self.server = server
        self.conn = conn
        self.addr = addr
        self.name = ""
        self.lock = lock
        t = threading.Thread(target=self.listen_to_messages)
        t.start()

    def send_message(self, msg):
        self.conn.sendall(msg.encode('ascii'))

    def is_name_valid(self, name):
        return not (not name)

    def listen_to_messages(self):
        while True:
            try:
                msg = self.conn.recv(1024)
            except socket.error:
                self.exit()
                break

            decoded_msg = msg.decode('ascii')

            if not self.name:
                if not self.is_name_valid(decoded_msg):
                    self.send_message('please choose a valid name')

                else:
                    self.name = decoded_msg
                    lock.acquire()
                    self.server.send_to_almost_all(self.name + " entrou na sala", self.addr)
                    lock.release()

            elif decoded_msg == 'sair':
                self.exit()
                break

            else:
                self.server.send_to_almost_all(self.name + " disse: " + decoded_msg, None)                

    def exit(self):
        lock.acquire()
        self.server.remove_client(self.addr)
        self.server.send_to_almost_all(self.name + " saiu da sala", self.addr)                
        lock.release()
        self.conn.close()

class Server:
    def __init__(self):
        self.clients = {}

    def add_client(self, addr, client):
        self.clients[addr] = client

    def remove_client(self, addr):
        del self.clients[addr]

    def send_to_almost_all(self, msg, client_not_to_send):
        for client_addr, client in list(self.clients.items()):
            if client_addr == client_not_to_send:
                continue

            client.send_message(msg)


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
#host = socket.gethostbyname(socket.gethostname())   
host = ''
port = args.port
lock = threading.Lock()                              

server = Server()

sock.bind((host, port))                                  
sock.listen(10)                                           

while True:
    conn, addr = sock.accept()
    client = ClientConn(server, conn, addr, lock)
    server.add_client(addr, client)
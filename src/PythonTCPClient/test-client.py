import socket
import threading
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("host", help="the address of the server")
parser.add_argument("port", help="the port of the server", type = int)
parser.add_argument("nick", help="your nickname")
args = parser.parse_args()



sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

host = args.host                           
port = args.port

sock.connect((host, port)) 
sock.send(args.nick.encode('ascii'))

def recv(sock):
    while True:
         data = sock.recv(1024)
         data = data.decode('ascii')
         print(data)

threading.Thread(target=recv, args=(sock,)).start()

while True:
    msg = input()
    sock.send(msg.encode('ascii'))
    if(msg == "sair"):
        sock.close()

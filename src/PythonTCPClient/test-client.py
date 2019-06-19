import socket
import threading
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("server_address", help="the address of the server")
parser.add_argument("port", help="the port of the server", type = int)
parser.add_argument("nick", help="your nickname")
args = parser.parse_args()



sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

host = args.server_address                           
port = args.port

sock.connect((host, port)) 
sock.send(args.nick.encode('ascii'))
print("Conectado ao servidor " + str(host) + " na porta " + str(port))

lock = threading.Lock()

def recv(sock):
    while True:
        try:
            data = sock.recv(1024)
            data = data.decode('ascii')
            print(data)
        except socket.error:
            sock.close()
            break

threading.Thread(target=recv, args=(sock,)).start()

while True:
    msg = input()

    try:
        sock.send(msg.encode('ascii'))

    except socket.error:
        sock.close()
        lock.release()
        break

    if(msg == "sair"):
        sock.close()
        break

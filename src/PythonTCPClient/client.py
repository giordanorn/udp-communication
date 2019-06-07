import socket

host = "127.0.0.1"
port = 5000
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

dest = (host, port)

sock.connect(dest)
sock.send( "diogo".encode() )
sock.send("oi".encode())
s = sock.recv(1024)
print(s)

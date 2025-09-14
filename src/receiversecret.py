import socket
import time

ip = "0.0.0.0"
port = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.bind((ip, port))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    now = time.time()
    print("received message: %s" % data)

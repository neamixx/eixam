import json
import socket
import time

ip = "10.17.88.219"  # Receiver IP
port = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP

peer_id = "peer1"  # Unique identifier for this sender

while True:
    stats = {
        "id": peer_id,
        "cpu": 50,     # Replace with real values if needed
        "ram": 30,
        "disk": 20,
        "timestamp": time.time()
    }

    message = json.dumps(stats).encode('utf-8')
    sock.sendto(message, (ip, port))
    print(f"Sent heartbeat from {peer_id}")
    time.sleep(5)

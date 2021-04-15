import socket
import sys

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('localhost', 8000)

try:
    # Send data
    message = "ping"
    sent = sock.sendto(message.encode(), server_address)
    data, server = sock.recvfrom(1024)
    print(data.decode())
finally:
    sock.close()
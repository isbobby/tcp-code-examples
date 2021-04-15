
import socket

localIP     = "127.0.0.1"
localPort   = 8000
BUFFER_SIZE  = 1024

# Define the message to send back
def get_response():
    response_string = """EE-4210: Continuous assessment"""
    return response_string
    

# Create a datagram socket

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# Bind to address and ip
UDPServerSocket.bind((localIP, localPort))
print(f"UDP server up and listening at {localPort}")
# Listen for incoming datagrams

while(True):
    message, address = UDPServerSocket.recvfrom(BUFFER_SIZE)
    # Sending a reply to client
    msgFromServer = get_response()
    bytesToSend = str.encode(msgFromServer)
    UDPServerSocket.sendto(bytesToSend, address)
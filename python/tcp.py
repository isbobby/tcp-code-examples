"""
 Implements a simple HTTP/1.0 Server

"""

import socket
from urllib.parse import unquote


# Define socket host and port
SERVER_HOST = '0.0.0.0'
SERVER_PORT = 8000

# HTML Form
def get_response(input_string):
    if len(input_string) == 0:        
        html_form = """
            <!DOCTYPE html>
            <head></head>
            <body>
                <form action="/display" method="GET">
                    <label for="usertext">Enter Text Here (press enter to send):</label><br>
                    <input type="text" id="usertext" name="usertext"><br>
                </form>
            </body>
        """
        return html_form
    else:
        formatted_string = input_string.replace('+', ' ')
        formatted_string = unquote(formatted_string)
        # HTML display
        html_display = f"""
            <!DOCTYPE html>
            <head></head>
            <body>
                <div>
                    You have entered {formatted_string}
                </div>
            </body>
        """
        return html_display
    


def get_endpoint(raw_string):
    # split by empty lines
    lines = raw_string.split('\n')
    
    # split by empty spaces
    tokens = lines[0].split(' ')
    return tokens[1]

def get_user_input(endpoint):
    # check if end point contains ?
    if '?' in endpoint:
        data = endpoint.split('?')
        user_string_tokens = data[1].split('=')
        return user_string_tokens[1]
    else:
        return ""    
    

# Create socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((SERVER_HOST, SERVER_PORT))
server_socket.listen(1)
print('Listening on port %s ...' % SERVER_PORT)

while True:    
    # Wait for client connections
    client_connection, client_address = server_socket.accept()

    # Get the client request
    request = client_connection.recv(1024)
    request_string = request.decode() 
    token = get_endpoint(request_string)

    user_input = get_user_input(token)

    response_string = get_response(user_input)

    # Send HTTP response
    response = f'HTTP/1.0 200 OK\n\n {response_string}' 
    client_connection.sendall(response.encode())
    client_connection.close()

# Close socket
server_socket.close()


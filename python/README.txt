1. Ensure python3 is installed on your local machine

TCP Server
1. Go to the directory where tcp.py is located
2. Run the server with 'python3 tcp.py'
3. The server will run on 127.0.0.1:8000
4. Open a browser and enter 127.0.0.1:8000 and test the response
5. Enter some text and hit enter, the response will be rendered as a HTML document
6. You can open multiple tabs to verify concurrent connections 

UDP Server
1. Go to the directory where udp.py is located
2. Run the server with 'python3 udp.py'
3. The server will run on 127.0.0.1:8000
4. Use telnet/echo to ping 127.0.0.1:8000 on terminals
5. The server will response a string (not in HTML format)
6. Open multiple terminals and ping to verify concurrency

Alternatively, a simple UDP client is provided to send request to server
1. Go to the directory where udp_client.py is located
2. Send a request to the server with python3 udp_client.py

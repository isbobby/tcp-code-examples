const net = require("net")

const server = net.createServer(
    socket => {
        socket.write("Hello, welcome to my TCP server\n"),
            socket.on("data", data => {
                console.log("Received byte buffer data: " + data.toString())
                socket.write("I can repeat that:" + data.toString())
            })
    }
)

server.listen(8080)

// simulate client message with telnet 127.0.0.1 8080
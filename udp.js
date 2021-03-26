const dgram = require('dgram')
const socket = dgram.createSocket('udp4');

socket.on('message', (msg, rinfo) => {
    console.log(`server has got ${msg} from ${rinfo.address}:${rinfo.port}`)
})

socket.bind(8081)

// simulate client message echo hi | nc -u 127.0.0.1 8081
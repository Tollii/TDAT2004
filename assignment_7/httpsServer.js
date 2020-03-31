'use strict';
const sha1 = require("crypto-js/sha1");
const Base64 = require("crypto-js/enc-base64");
const websocket = require('./websocket');
const net = require('net');
const fs = require('fs');

// Simple HTTP server responds with a simple WebSocket client test
const httpServer = net.createServer(connection => {
  connection.on('data', () => {
    const content = fs.readFileSync('./index.html');
    connection.write('HTTP/1.1 200 OK\r\nContent-Length: ' + content.length + '\r\n\r\n' + content);
  });
});
httpServer.listen(3000, () => {
  console.log('HTTP server listening on port 3000');
});


const ws = new websocket();
ws.listen(3001);

setTimeout( () => {
    ws.emit(`Right what's all this then`);
}, 5000);
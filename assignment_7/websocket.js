'use strict';
const sha1 = require("crypto-js/sha1");
const Base64 = require("crypto-js/enc-base64");
const net = require('net');

module.exports = class websocket {
    constructor(port) {
        this.connections = [];
        this.wsServer = net.createServer(connection => {
            this.onData(connection);
            this.closeConnection(connection);
        });

        this.wsServer.on('error', error => {
              console.error('Error: ', error);
        });
    }

    listen = port => {
        this.wsServer.listen(port, () => {
            console.log(`websocket server listening on port ${port}\n`);
        });
    };

    emit = str => {
        const output = this.frameData(str);
        for(let i = 0; i < this.connections.length; i++) {
            this.connections[i].write(output);
        }
    };

    onData = connection => {
        connection.on("data", data => {
            if (data.toString().includes("HTTP/1.1")) {
                this.handShake(data, connection);
            } else {
                this.payLoadPackets(data, connection);
            }
        });
    };

    payLoadPackets = (data, connection) => {

        for (let i = 0; i < this.connections.length; i++) {

            if (Buffer.from(data)[0] === 136) {
                continue;
            }

            if (this.connections[i] !== connection) {
                const msg = this.decode(data);
                const frame = this.frameData(msg);
                this.connections[i].write(frame);
            }
            // write to self
            else if (this.connections[i] === connection) {
                connection.write(this.frameData("Data sent"));
            }
        }
    };

    closeConnection = connection => {
        connection.on('end', () => {
			console.log('Client disconnected');

			//Remove connection from array
			this.connections = this.connections.filter(con => con != connection);

			//Sends a FIN packet
			connection.end();
		});
    };

    handShake = (data, connection) => {
        console.log('Client connected\n');
        console.log('Data received from client: ', data.toString());

        if (this.connections.indexOf(connection) === -1)
			this.connections.push(connection);
        

        const headers = data.toString().split("\n");

        let key = "";
        headers.map(header => {
            if (header.indexOf("Sec-WebSocket-Key:") > -1) {
                const indexStart = header.indexOf("Key:");
                key = header.substring(indexStart + 4).trim();
            }
        });

        //Hash with SHA1 and Encode entire string with Base64
        const hash = sha1(key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
        const encoded = Base64.stringify(hash);

        //Return header message to client
        connection.write("HTTP/1.1 101 Switching Protocols\r\n");
        connection.write("Upgrade: websocket\r\n");
        connection.write("Connection: Upgrade\r\n");
        connection.write(`Sec-WebSocket-Accept: ${encoded.trim()}\r\n`);
        connection.write("\r\n");
    };


    decode = hex => {
        let msg = "";

        // Hex in form of byte array
        const bytes = Buffer.from(hex);

        const length = bytes[1] & 127;
        const maskStart = 2;
        const dataStart = maskStart + 4;
        for (let i = dataStart; i < dataStart + length; i++) {
            // Unmasks bytes and add character
            const byte = bytes[i] ^ bytes[maskStart + ((i - dataStart) % 4)];
            msg += String.fromCharCode(byte);
        }

        return msg;
    };

	// Formats data correctly
    frameData = str => {
		
		let frame = [];

		frame.push(0x81);

		if (str.length > 127)
			return frameData("max of 127 bytes");

		/*
			Second byte is maskedAndLength
			We do not mask our data from the server. Leftmost bit would be 1 if we did.
		*/
		let maskedAndLength = 0b00000000 | str.length;
		frame.push(maskedAndLength);

		const payload = Buffer.from(str);

		for (let i = 0; i < payload.length; i++) {
			frame.push(payload[i]);
		}

		//Return a byte arrays of frame
		return Buffer.from(frame);
	}
};
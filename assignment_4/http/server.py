import socket
import threading
import json
import os
import re


class Server(threading.Thread):
	def __init__(self, client_address, client_socket):
		threading.Thread.__init__(self)
		self.csocket = client_socket
		print("New connection added: ", client_address)

	def run(self):
		print("Connection from : ", client_address)
		
		msg = ''
		with open('index.html', 'r') as file:
			for line in file: 
				msg += line

		msg += '<ul> <li>'
		header = str(self.csocket.recv(2048))
		list = re.sub(r'\\n', '</li> <li>', header)
		w = re.sub(r'\\r', '', list)
		print(w)
		msg += w
		msg += '</li></ul>'

		self.csocket.sendall(str.encode("""HTTP/1.0 200 OK\n""",'iso-8859-1'))
		self.csocket.sendall(str.encode('Content-Type: text/html\n', 'iso-8859-1'))
		self.csocket.send(str.encode('\n'))
		self.csocket.send(str.encode(msg))
		self.csocket.shutdown(1)
		
		print("Client at ", client_address, " disconnected...")


if __name__ == '__main__':

	IP = 'localhost'
	PORT = 8080
	server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 5)
	server.bind((IP, PORT))

	print("Server started")
	print("Waiting for client request..")

	while True:
		server.listen(5)
		client_socket, client_address = server.accept()
		newthread = Server(client_address, client_socket)
		newthread.start()

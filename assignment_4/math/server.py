import socket
import threading
import json


class Server(threading.Thread):
	def __init__(self, client_address, client_socket):
		threading.Thread.__init__(self)
		self.csocket = client_socket
		print("New connection added: ", client_address)


	def add(self, num1, num2):
		return str(num1 + num2)

	def sub(self, num1, num2):
		return str(num1 - num2)

	def run(self):
		print("Connection from : ", client_address)
		while True:
			data = self.csocket.recv(2048)
			msg = data.decode()
			try: 
				json_encoded = json.loads(msg)
			except Exception:
				continue

			func = json_encoded['func']
			num1 = int(json_encoded['num1'])
			num2 = int(json_encoded['num2'])

			if func == 'add':
				res = str(self.add(num1, num2))
			elif func == 'sub':
				res = str(self.sub(num1, num2))
			else:
				res = 'Something went wrong...'

			print("from client", json_encoded)
			self.csocket.send(bytes(res, 'UTF-8'))
		print("Client at ", client_address, " disconnected...")


if __name__ == '__main__':

	IP = 'localhost'
	PORT = 8080
	server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	server.bind((IP, PORT))

	print("Server started")
	print("Waiting for client request..")

	while True:
		server.listen(1)
		client_socket, client_address = server.accept()
		newthread = Server(client_address, client_socket)
		newthread.start()

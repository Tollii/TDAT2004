
import socket
import json

SERVER = 'localhost'
PORT = 8080
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((SERVER, PORT))
# client.sendall(bytes({}))

while True:

	print('\n"add" for addition, "sub" for subtraction... Type "quit" to exit')
	func = input()

	if func == 'quit':
		break

	if func != 'add' and func != 'sub':
		raise ValueError

	print("First number...")
	num1 = int(input())

	print("Second number...")
	num2 = int(input())

	data = {
		'func': func,
		'num1': num1,
		'num2': num2
	}

	json_data = json.dumps(data)

	client.sendall(bytes(json_data,'UTF-8'))
	in_data = client.recv(1024)
	print("From Server :", in_data.decode())
	client.close()

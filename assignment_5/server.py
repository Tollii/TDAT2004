import socket
import json 

def add(num1, num2):
		return str(num1 + num2)

def sub(num1, num2):
	return str(num1 - num2)


UDP_IP = "127.0.0.1"
UDP_PORT = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

data, addr = sock.recvfrom(1024)
json_data = json.loads(data.decode())

func = json_data['func']
print(func)
num1 = int(json_data['num1'])
print(num1)
num2 = int(json_data['num2'])
print(num2)

res = ''
if func == 'add':
	res = str(add(num1, num2))
elif func == 'sub':
	res = str(sub(num1, num2))
else:
	res = 'Something went wrong...'


print(f'Sending response: {res}')

sock.sendto(res.encode(), (UDP_IP, UDP_PORT))
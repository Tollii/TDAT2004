import socket
import json 

UDP_IP = "127.0.0.1"
UDP_PORT = 5005
MESSAGE = "Hello, World!" 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# sock.bind((UDP_IP, UDP_PORT))
sock.connect((UDP_IP, UDP_PORT))

print(f'Server IP: {UDP_IP}')
print(f'Server port: {UDP_PORT}')

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
    sock.sendall(json_data.encode())
    data, addr = sock.recv(1024)

    print(f'Recieved from server: {data.decode()}')
    sock.close()
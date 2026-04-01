import socket
import threading
import sys
import argparse
from datetime import datetime, timedelta

#TODO: Implement all code for your server here

clients = {}
clients_lock = threading.Lock()
password = ""

def broadcast(message, sender_socket):
    with clients_lock:
        for client_socket, username in clients.items():
            if client_socket != sender_socket:
                try:
                    client_socket.send(message.encode('utf-8'))
                except:
                    pass

def broadcast_to_all(message):
    with clients_lock:
        for client_socket in clients:
            try:
                client_socket.send(message.encode('utf-8'))
            except:
                pass

def handle_client(client_socket):
    client_file = client_socket.makefile('r')
    username = ""

    try:
        username = client_file.readline().strip()
        passcode = client_file.readline().strip()
        
        if not username or not passcode:
            return 
        if passcode != password:
            client_socket.send("Incorrect passcode\n".encode('utf-8'))
            return 
        client_socket.send("OK\n".encode('utf-8'))
        
        with clients_lock:
            clients[client_socket] = username
        
        print(f"{username} joined the chatroom")
        sys.stdout.flush()
        broadcast(f"{username} joined the chatroom", client_socket)

        for message in client_file:
            message = message.strip() 
            
            if not message:
                break 

            if message == ':)':
                response = f"{username}: [feeling happy]"
                print(response)
                sys.stdout.flush()
                broadcast(response, client_socket)
            elif message == ':(':
                response = f"{username}: [feeling sad]"
                print(response)
                sys.stdout.flush()
                broadcast(response, client_socket)
            elif message == ':mytime':
                time_str = datetime.now().strftime('%a %b %d %H:%M:%S %Y')
                response = f"{username}: {time_str}"
                print(response)
                sys.stdout.flush()
                broadcast_to_all(response)
            elif message == ':+1hr':
                time_str = (datetime.now() + timedelta(hours=1)).strftime('%a %b %d %H:%M:%S %Y')
                response = f"{username}: {time_str}"
                print(response)
                sys.stdout.flush()
                broadcast_to_all(response)
            elif message == ':Users':
                print(f"{username}: searched up active users")
                sys.stdout.flush()
                with clients_lock:
                    user_list = ", ".join(clients.values())
                response = f"Active Users: {user_list}"
                client_socket.send(response.encode('utf-8')) 
            elif message.startswith(':Msg'):
                parts = message.split(' ', 2)
                if len(parts) >= 3:
                    target_user, private_msg = parts[1], parts[2]
                    print(f"{username}: send message to {target_user}")
                    sys.stdout.flush()
                    target_socket = None
                    with clients_lock:
                        for sock, user in clients.items():
                            if user == target_user:
                                target_socket = sock
                                break
                    if target_socket:
                        formatted_pm = f"[Message from {username}]: {private_msg}"
                        target_socket.send(formatted_pm.encode('utf-8'))
            elif message == ':Exit':
                break
            else:
                response = f"{username}: {message}"
                print(response)
                sys.stdout.flush()
                broadcast(response, client_socket)

    except (ConnectionResetError, BrokenPipeError):
        pass
    finally:
        broadcast_message = None 
        
        with clients_lock:
            if client_socket in clients:
                username = clients.pop(client_socket) 
                print(f"{username} left the chatroom")
                sys.stdout.flush()
            
                broadcast_message = f"{username} left the chatroom"

        if broadcast_message:
            broadcast_to_all(broadcast_message)
        
        client_file.close() 
        client_socket.close()


def main():
    global password
    parser = argparse.ArgumentParser()
    parser.add_argument('-start', action='store_true')
    parser.add_argument('-port', type=int, required=True)
    parser.add_argument('-passcode', type=str, required=True)
    args = parser.parse_args()

    password = args.passcode
    port = args.port
    host = '127.0.0.1' 

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server_socket.bind((host, port))
        server_socket.listen(10)
        print(f"Server started on port {port}. Accepting connections")
        sys.stdout.flush()

        while True:
            client_socket, addr = server_socket.accept()
            thread = threading.Thread(target=handle_client, args=(client_socket,))
            thread.start()

    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        server_socket.close()

if __name__ == "__main__":
    main()
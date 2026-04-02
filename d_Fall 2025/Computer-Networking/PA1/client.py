import os
import socket
import threading
import sys
import argparse

# TODO: Implement a client that connects to your server to chat with other clients here

stop_thread = False

def receive_messages(client_socket):
    while not stop_thread:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if message:
                if message == "Incorrect passcode":
                    print(message)
                    sys.stdout.flush()
                    os._exit(1)
                else:
                    print(message)
                    sys.stdout.flush()
            else:
                break
        except:
            break

def main():
    global stop_thread
    parser = argparse.ArgumentParser()
    parser.add_argument('-join', action='store_true')
    parser.add_argument('-host', type=str, required=True)
    parser.add_argument('-port', type=int, required=True)
    parser.add_argument('-username', type=str, required=True)
    parser.add_argument('-passcode', type=str, required=True)
    args = parser.parse_args()

    if not args.join:
        print("Please use the -join flag to run the client.")
        sys.exit(1)

    host = args.host
    port = args.port
    username = args.username
    passcode = args.passcode

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        client_socket.connect((host, port))

        client_socket.send(f"{username}\n".encode('utf-8'))
        client_socket.send(f"{passcode}\n".encode('utf-8'))
        client_file = client_socket.makefile('r')
        response = client_file.readline().strip()

        if response == "Incorrect passcode":
            print(response)
            sys.stdout.flush()
            return 
        elif response != "OK":
            print(f"Unexpected response from server: {response}")
            return
        
        print(f"Connected to {host} on port {port}")
        sys.stdout.flush()

        receiver_thread = threading.Thread(target=receive_messages, args=(client_socket,))
        receiver_thread.start()

        for line in sys.stdin:
            message = line.strip()
            if message:
                client_socket.send(f"{message}\n".encode('utf-8'))
            
            if message == ':Exit':
                break

    except Exception as e:
        print(f"Could not connect to the server: {e}")
    finally:
        stop_thread = True
        client_socket.close()

if __name__ == "__main__":
    main()
                
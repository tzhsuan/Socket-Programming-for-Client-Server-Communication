# UDP Client-Server Implementation in C

## Overview
This repository contains a simple UDP client-server implementation in C, demonstrating basic socket programming for UDP communication. The server listens for incoming messages on port `12345`, receives messages from the client, and responds with a predefined message. The client sends a message to the server and waits for a response. The implementation uses the `htons()` function to ensure proper byte order conversion for cross-platform compatibility.

## Features
- **Server** (`server.c`):
  - Creates a UDP socket and binds to port `12345` on all available network interfaces (`INADDR_ANY`).
  - Receives messages from clients using `recvfrom()`.
  - Sends a response ("Hello from server") using `sendto()`.
- **Client** (`client.c`):
  - Creates a UDP socket and sends a message ("Hello from client") to the server at `192.168.1.184:12345`.
  - Receives the server's response using `recvfrom()`.
- Uses `htons()` to convert port numbers to network byte order (big-endian) for compatibility across systems.
- Includes error handling for socket creation, binding, address conversion, and message transmission.

## Prerequisites
- A C compiler (e.g., `gcc`).
- Linux/Unix-based system or Windows with a compatible socket library (e.g., Winsock for Windows).
- Required headers: `<stdio.h>`, `<arpa/inet.h>`, `<netinet/in.h>`, `<string.h>`, `<sys/socket.h>`, `<sys/types.h>`, `<unistd.h>`.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/tzhsuan/Socket-Programming-for-Client-Server-Communication
   cd Socket-Programming-for-Client-Server-Communication
   ```
2. Compile the server and client programs:
   ```bash
   gcc -o server server.c
   gcc -o client client.c
   ```

## Usage
1. **Run the server**:
   ```bash
   ./server
   ```
   The server listens on port `12345` for incoming UDP messages.

2. **Run the client**:
   ```bash
   ./client
   ```
   The client sends a message to the server at `192.168.1.184:12345` and waits for a response.

   **Note**: Update the IP address in `client.c` (`192.168.1.184`) to match the server's IP address if running on different machines.

3. **Test with netcat (optional)**:
   Instead of the client program, you can use `netcat` to send a message to the server:
   ```bash
   echo "Hello, Server!" | nc -u 127.0.0.1 12345
   ```

## Code Explanation
- **Server (`server.c`)**:
  - Creates a UDP socket using `socket(AF_INET, SOCK_DGRAM, 0)`.
  - Binds to port `12345` using `bind()` with `INADDR_ANY` to accept messages from any interface.
  - Uses `htons(12345)` to convert the port number to network byte order.
  - Receives client messages with `recvfrom()` and responds with `sendto()`.

- **Client (`client.c`)**:
  - Creates a UDP socket and configures the server address (`192.168.1.184:12345`).
  - Uses `inet_pton()` to convert the human-readable IP address to binary form.
  - Uses `htons(12345)` to convert the port number to network byte order.
  - Sends a message to the server with `sendto()` and receives the response with `recvfrom()`.

- **Error Handling**: Both programs check for errors in socket creation, binding, address conversion, and message transmission, printing appropriate error messages with `perror()`.

## Example Output
**Server** (when receiving a message from the client):
```
Client (received 17 bytes): Hello from client
Message sent, total 17 bytes.
```

**Client** (after sending a message to the server):
```
Message sent, total 17 bytes.
Client (received 17 bytes): Hello from server
```

## Notes
- The server uses `INADDR_ANY` to bind to all network interfaces, making it accessible from any IP address on the host machine.
- The client is hardcoded to connect to `192.168.1.184:12345`. Ensure the server is running on the correct IP and update the client code if necessary.
- The `htons()` function ensures port numbers are correctly interpreted in network byte order, critical for cross-platform compatibility.
- The current implementation handles one message exchange and exits. For continuous communication, consider adding a loop to handle multiple messages.

## Troubleshooting
- **IP Address**: Update the IP address in `client.c` to match the server's actual IP address.
- **Firewall**: Ensure UDP traffic is allowed on port `12345`.
- **Run Server First**: The server must be running before the client sends a message.
- **Port Conflict**: Ensure no other application is using port `12345`.

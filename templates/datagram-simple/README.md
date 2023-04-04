# Connectionless Client-Server Communication

This is a client-server application where the client requests a unique number from the server. The project uses Unix domain sockets for communication. The server uses a socket to listen for incoming requests from the client. Once a request is received, the server sends back the next number in sequence. On the other hand, the client sends an empty packet as a trigger and waits for the server to respond with the next number.

## Datagram-based Messaging

Both, server and client, are implemented as connectionless. Such datagram-based protocols are typically used in situations where small amounts of data need to be transmitted quickly and efficiently, such as in real-time applications like gaming or voice chat.

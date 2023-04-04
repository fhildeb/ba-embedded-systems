# TCP Client-Server Network Communication

This project contains a client and a server, that communicate with each other over a TCP connection. The client sends text lines to the server, which then converts them to uppercase and sends them back to the client for display.

The server can handle up to two simultaneous connections. Any additional connection will be rejected. The client program takes a server address as a command-line argument, while the server program uses the default port number specified in the config header file.

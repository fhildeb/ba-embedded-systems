# Domain Socket Stream Server

This project consists of a server and client that communicate over a Unix domain socket to provide unique numbers to clients. The server can be run in either iterative or parallel mode and sends the next available number to a client upon connection. The client connects to the server and reads the number sent by the server. The project includes error handling and signal handling to ensure reliable operation.

## Iterative Server

`iterative-server` is an iterative stream server. It waits for a client connection, sends the next available number, and then waits for the next client to connect. It handles only one client at a time, and waits for the current client to disconnect before serving the next.

`parallel-server`, on the other hand, is a parallel stream server that handles multiple clients simultaneously. It forks a child process for each new client connection, sends the next available number to the client, and then waits for another client connection. The child processes handle the communication with each client independently.

## Usage

To test the programs, run `client` in one, and either `parallel` or `iterative server` in another terminal.

# Multi-Thread File Transfer

This program is a multithreaded file transfer application that utilizes Unix domain sockets for communication between the client and server.

The client reads a source file and sends it in blocks to the server, which then writes the received data to a destination file. The server can handle multiple client connections simultaneously using separate threads. Error handling and response mechanisms are in place to ensure proper communication and file transfer between the client and server.

## Usage

```bash
# Start server
./server

# Start client and pass
# source file path and name
# destination file path and name
./client source_file_path destination_file_path

# Example
./client source.txt destination.txt
```

# Network File Transfer

This project is a file transfer application that allows users to send files from a client to a server over a network. The client connects to the server by providing the server's IP address, source file path, and destination file path as arguments. The server listens for incoming connections and saves the transferred files at the specified location. The client and server communicate using custom-defined message types, error handling, and a simple protocol for transferring file data in blocks.

## Usage

```bash
# Start the server
./server

# Start the client and pass
# server IP address
# source file path and name
# destination file path and name
./client server_address source_file destination_file

# Example
./client 192.168.x.x ./source.txt ./destination.txt
```

## Local Testing

To determine your machine's IP address for local testing use:

```bash
ifconfig | grep "inet "
```

Look for the local IP address that belongs to your machine within the network.
It usually starts with `192.168.x.x` or `10.x.x.x`.
This can be used your server's IP address.

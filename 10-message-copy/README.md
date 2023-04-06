# Message Queue File Transfer

This project implements a client-server file transfer system using message queues for inter-process communication. The client reads a source file and sends its contents to the server in blocks, which then writes the data to a destination file. The client and server programs use message queues to exchange data, including the destination file's name, size, and data blocks. The server responds with success or error messages to indicate the success of each operation. It also handles signals and installs exit handlers to clean up resources, such as message queues, upon termination.

## Usage

```bash
# Start the server
./server

# Start the client and pass the arguments
# servername: a sample file on your machine, used to sync message queues from forks
# source_file: path and name of file to copy
# destination_file: destination path and name of copied file
 ./client servername source_file destination_file

# Example
./client ./sample.txt ./copy.md ./destination.md
```

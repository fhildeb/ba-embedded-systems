# Unix Socket File Transfer

This project is a file transfer application that allows users to copy files from a source directory to a destination directory on the same machine. It utilizes Unix domain sockets to handle the communication between the client and server program.

The client reads the source file, sends the file information, and transmits the file data in packages to the server, which then writes the received data to the destination file. The server listens for incoming connections and handles file creation and data writing, while the client program is responsible for reading and sending the source file's data. The communication protocol includes success, error, and bye messages to ensure proper data transfer and error handling.

## Usage

```bash
# Start the server
./server

# Start the client and pass
# source file path and name
# destination file path and name
./client ./source_file ./destination_file

# Example
./client sample.txt copy.txt

```

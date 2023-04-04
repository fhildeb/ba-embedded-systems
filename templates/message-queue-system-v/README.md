# Client-Server Communication using System V Message Queues

This project shows inter-process communication between a client and a server using message queues. The client sends requests to the server for trigonometric functions (sin, cos, and tan) to be performed on a given argument. The server receives the requests, performs the requested function, and sends back the result to the client.

The client and server both include the msg.h header file, which defines the message types and structures used in the communication.

The program handles errors and signals such as `SIGINT`, which is caught by the signal handler and causes the message queue to be deleted before the program exits.

## Messaging with System V

The server defines a message template containing structures for an order and a result, and creates a message queue using the get message function from System V. It then enters a loop where it waits for incoming messages using the receive message function, performs the job, and sends back the result using the send message function.

System V is a specific version of Unix that includes message operations. While providing a monolithic kernel and being developed in 1970, System V message queues are still widely used and are supported on a wide range of operating systems.

# Client-Server Communication using POSIX Message Queues

This project is a client-server communication program using message queues. The client sends a request for a trigonometric function (sinus, cosinus, tangens) to be calculated on an argument to the server through a message queue. The server calculates the requested function and sends the result back to the client through a separate message queue. The communication between the client and server is implemented using POSIX message queues.

The message queue implementation ensures reliable communication between the client and server while avoiding common problems like deadlocks and buffer overflow. It also allows multiple clients to connect to the server at the same time, each having its separate message queue to receive results.

## Messaging using POSIX

The server creates a message queue using the open message queue function and waits for incoming messages using the receive message function from POSIX. It then performs the requested function and sends back the result using the send message function.

POSIX is a set of standards that can be implemented on any Unix system. Its is a modern and modular way of introducing message queues and in general much more user-friendly than others, including the System V variant.

# Inter-Process Communication with Shared Memory and Semaphores

This program demonstrates interprocess communication between a client and a server using shared memory and semaphores.

The server creates a shared memory segment and a semaphore group consisting of 3 semaphores, initializes them, and waits at semaphore 1 for a message from the client. It there is a message, the server processes the message and signals semaphore 2, to indicate that a response has been deposited in the shared memory segment. The server then releases exclusive access to the shared memory segment. The program ends when the client sends a message starting with "0".

The client waits for exclusive access to the server using semaphore 0, writes a message to the shared memory segment, and signals semaphore 1 to indicate that the message has been deposited in the shared memory segment.

- Semaphore 0: Setting mutual exclusion between the client and the server
- Semaphore 1: Signaling the server that a client has written a message
- Semaphore 2: Signaling the client that the server has processed the message

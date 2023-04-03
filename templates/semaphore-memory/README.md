# Inter-Process Communication with Shared Memory and Semaphores

This program demonstrates interprocess communication between a client and a server using shared memory and semaphores.

The server creates a shared memory segment and a semaphore group consisting of 3 semaphores, initializes them, and waits at semaphore 1 for a message from the client. It there is a message, the server processes the message and signals semaphore 2, to indicate that a response has been deposited in the shared memory segment. The server then releases exclusive access to the shared memory segment. The program ends when the client sends a message starting with "0".

The client waits for exclusive access to the server using semaphore 0, writes a message to the shared memory segment, and signals semaphore 1 to indicate that the message has been deposited in the shared memory segment.

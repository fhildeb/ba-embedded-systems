# Inter-Process Communication on Shared Memory

This project demonstrates the use of shared memory for inter-process communication between a client and server. The server creates and shares a memory segment and the client accesses it by mapping it into its own address space. The server installs an exit handler to clean up the shared memory segment in case of abnormal termination. The program includes a signal handler to remove the shared memory segment in case of an interrupt signal.

> The code does not include any synchronization mechanisms, making it prone to race conditions.

# Inter-Process Communication with Memory Mapping

This project shows how to implement interprocess communication using memory mapping. The writer program creates a shared memory space, writes some data into it, and waits for user input before exiting. The reader program then opens the shared memory space, reads the data from it, and prints it to the console.

The two programs are connected through the shared memory space, allowing them to communicate with each other. This method of interprocess communication is efficient and avoids the overhead of message passing.

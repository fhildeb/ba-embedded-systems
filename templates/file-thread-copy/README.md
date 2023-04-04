# Multi-Threaded File Copying

This program copies a file using two threads: the producer thread reads data from the source file and writes it to a shared buffer, while the consumer thread reads the buffer and writes to the destination file.

The program uses mutexes and condition variables to synchronize access to the buffer and ensure that the producer and consumer threads take turns using it. If the destination file already exists,

the program prompts the user to confirm overwriting it.

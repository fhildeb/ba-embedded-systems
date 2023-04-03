# Thread-Specific Data with Keys

This code snippet demonstrates how to use thread-specific data with keys. TSD allows each thread to have its own private copy of data, which is initialized and cleaned up automatically by the operating system.

This program creates three threads that increment a counter independently and print out its value to the console every second. The program uses thread-specific data (TSD) to ensure that each thread has its own copy of the counter, initialized to zero. The counter is then incremented each time the thread executes its code. The program also uses a key-value pair to store the TSD, and a cleanup function to free the memory allocated for the counter when the thread terminates.

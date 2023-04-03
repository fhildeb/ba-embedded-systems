# Simple Thread Setup

This program creates two threads that attempt to access a critical section protected by a mutex. The threads lock the mutex, print out a message to the console, wait for user input, unlock the mutex, and print out another message. As there is no synchronization used, the order in which the threads execute is not predictable, and may vary each time the program is run.

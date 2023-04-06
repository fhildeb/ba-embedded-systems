# Signal-based Process Synchronization

This program creates a child process using the forking system call and establishes a signal handler for the `SIGUSR1` signal. The parent and child processes take turns printing their process IDs 20 times, with the parent process starting first. They use the `SIGUSR1` signal to synchronize their execution.

After each print, a process sends a `SIGUSR1` signal to the other process and then waits for the signal from the other process before printing again. This ensures that the PIDs are printed in alternating order. Once the printing is completed, the parent process waits for the child process to terminate using the wait for process ID system call.

# Fork Termination

This program shows process management by creating a child process using the fork functionality. The parent process sleeps for 2 seconds before terminating, while the child process prints the parent's process ID twice: once immediately and once after a 4-second sleep. As the parent process ends before the child process, the second time the child prints the parent's PID, it will show a different value because the original parent process is no longer running, and the child process has been adopted by the init process (or another process depending on the system).

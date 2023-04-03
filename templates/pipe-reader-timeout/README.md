# Multi-Pipe Reader with Timeout

This program reads data from three named pipes (FIFOs) simultaneously using the select function to wait for input on any of them with a timeout of one second. The program opens the three FIFOs specified as command-line arguments, and adds them to a file descriptor set. Whenever input is available on any of the FIFOs, the program reads all available data and prints it to `stdout` with the corresponding FIFO label. The program exits when an 'E' character is received on any of the FIFOs or after the timeout period has elapsed.

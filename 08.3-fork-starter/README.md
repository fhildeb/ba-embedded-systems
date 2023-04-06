# Process Starter with Arguments

This program is a process starter that launches other programs passed as command-line arguments. When executed, it creates a child process using forking and then replaces the child process's image with the desired program using the execute vector path functionality. The parent process waits for the child process to finish and then prints the child's process ID and exit status.

The program accepts command-line arguments and initializes the child process's argument vector accordingly. It demonstrates basic process management and interaction between parent and child processes in C.

## Usage

```bash
# Run another program without arguments
./starter PATH_TO_PROGRAM

# Run another program with a single argument
./starter PATH_TO_PROGRAM arg1

# Run another progtram with multiple arguments
./starter PATH_TO_PROGRAM arg1 arg2 arg3
```

# Pipe Handling

Demonstrating inter-process communication using pipes.

## Pipe Duplication

> File: `pipe-duplicate.c`

This program creates a pipe and a child process using the fork functionality. The parent process writes to the pipe, while the child process reads from it and redirects the pipe to `STDIN_FILENO` using the duplication functionality. The child process then executes another program, `pipe-sub-input`, using the execution list.

## Pipe Execute

> File: `pipe-execute.c`

This program creates a pipe and a child process using fork. The parent process writes to the pipe, while the child process reads from it. The program uses the read and write system calls for input and output operations, and the fork system call to create a child process. The child process then executes another program, `pipe-sub-argument`, using the execution list and passes the pipe descriptor as a parameter.

## Pipes with Fork

> File: `pipe-fork.c`

This program creates a pipe and a child process using the fork functionality. The child process reads from the pipe, while the parent process writes to it.

## Pipe Opening and Closing

> File: `pipe-open-close.c`

The program showcases how pipe opening and closing can be used to execute shell commands and capture their output within a C program. It uses pipe opening functionality to execute the shell command `ps -aux` and read its output as a stream. It then reads and processes the output line by line and prints it to the console using.

## Simple Pipe

> File: `pipe-simple.c`

This program writes to a pipe, then reads the data from the pipe and displays it on the console.

# Subcommands

## Pipe using an Argument Vector

> File: `pipe-sub-argument.c`

This program reads data from a pipe inherited from the parent process using the file descriptor passed as a command-line argument to the program. The program uses the read system call to read the data from the pipe and displays it on the console.

## Pipe with Input

> File: `pipe-sub-input.c`

This program reads data from the standard input using the read system call and displays it on the console. The program is designed to be used as a child process that is spawned by another program using pipess.

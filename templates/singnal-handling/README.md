# Signal Handling

## Signal Masking and Blocking

> File: `signal-blocking.c`

This program demonstrates signal masking and checking for pending signals, with an example of blocking the `SIGINT` signal for a certain period of time. The program sets a handler for `SIGINT` signals to block and then checks if the `SIGINT` signal is pending. Afterward, it unblocks the `SIGINT` signal and waits for any pending signals before exiting.

## Delay Signals

> File: `signal-delay.c`

This program demonstrates how to install a signal handler for `SIGINT` and shows that blocked signals do not have queues. The program sets up a signal action structure with a handler function and installs it for the `SIGINT` signal using sigaction(). The program then enters an infinite loop where it waits for signals. When a `SIGINT` signal is received, the handler function is called and prints a message, then enters a "dead time" of 2 seconds to simulate a delay. If multiple `SIGINT` signals are received during the delay, the handler function is only called once after the delay.

## Install SIgnal Handler

> File: `signal-handler.c`

This program installs a signal handler for `SIGINT` and catches the first signal, then resets to the standard reaction. The program uses the signal action system call to set up a new signal handler, which catches `SIGINT` and prints a message. The `SA_ONESHOT` flag ensures that the handler only catches the first signal before resetting to the standard reaction. The program then enters an infinite loop, waiting for signals.

## Ignore Signals

> File: `signal-ignore.c`

This program shows how to disable and enable signal handling for a signal using the signal action system call. It creates a signal action structure to ignore the `SIGINT` signal and disables the signal handling for `SIGINT` using signal action. After sleeping for 5 seconds, the program restores the old signal handling and prints a message that `SIGINT` is enabled.

## Signal Jumping

> File: `signal-jumping.c`

This program demonstrates the use of singla jump functions to perform non-local jumps. The program sets up signal handlers for `SIGUSR1` and `SIGALRM`, and uses the signature set jump function to jump back to the main function in response to a `SIGUSR1` signal.

## Extended Signal Catching

> File: `signal-multicatch.c`

This program installs a signal handler for all signals except for `SIGKILL`. When a signal is caught, it exits the program with a failure status. The program also uses ANSI escape codes to change the output color and sets up an exit function to clean up before exiting the program.

## Detect Child Process Termination

> File: `signal-termination.c`

This program demonstrates how to create child processes and use the `SIGCLD` signal to detect their termination. The program creates ten child processes that sleep for two seconds and exit. The parent process pauses and waits for the `SIGCLD` signal to be received, which indicates that a child process has terminated. The signal handler then uses the wait function to fetch the child process's status and prints it. The program repeats the process for all child processes and then exits.

## Timeout Handling for Signals

> File: `signal-timeout.c`

This program shows how to handle a timeout signal. The program first sets up a signal action structure and installs it for the `SIGALRM` signal. It then sets up a buffer to return to after the timeout signal is caught by the handler function. The program sets an alarm for 5 seconds and prompts the user to input a word. If the user inputs a word before the timeout, the program turns off the alarm, and the word is printed. If the timeout is reached, the program jumps back to the buffer, sets a custom word, and prints it.

# Non-Blocking Input in Raw Mode

This program demonstrates how to read non-blocking input from the user in raw mode using the termios library. It sets the terminal to raw mode, which turns off canonical processing and echoes. It then sets the standard input to non-blocking mode using the file control function to allow the program to check for input without blocking.

The program repeatedly reads a single character from standard input and checks if input is available using the read function. If input is not available, it prints an asterisk and waits for one second before checking again. If input is available, it prints the character to standard output. The loop continues until the user enters `E`.

Once the loop is complete, the program restores the standard input to blocking mode and resets the terminal to its original state.

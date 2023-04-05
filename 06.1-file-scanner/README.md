# File and Directory Scanner

This program displays the file properties of a given file or directory, passed as a command-line argument. The program checks for the correct number of command-line arguments and determines if the input is a file or a directory.

For a single file, it prints out the properties, including name, device number, i-node number, link counter, data type, access rights, user information, group information, file size, and timestamps. If the input is a directory, the program iterates through the files within the directory and displays their properties in a similar manner.

The program uses functions from the standard C library for file handling, user and group information retrieval, and time manipulation.

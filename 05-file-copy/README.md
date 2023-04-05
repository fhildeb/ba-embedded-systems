# File Copy

This program is designed to copy the contents of one source file to another destination file, maintaining the same file permissions. The program uses the standard C library functions for file input/output and file handling. It checks for the correct number of command-line arguments and ensures that the source file exists and has read permissions. The program then opens or creates the destination file, modifying its permissions to match those of the source file. Using a buffer, the program reads the contents of the source file and writes them to the destination file. Once the copying process is complete, it closes both files and exits with an 'EXIT_SUCCESS' (0) status code.

## Features

- Uses read-write loop
- Defines a buffer of size 1024 bytes
- Copying process continues until file end

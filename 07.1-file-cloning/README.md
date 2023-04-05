# File Cloning

This project copies the content and permissions of one source file to another destination file. The program is a single file that uses standard system libraries and functions for handling file I/O, memory mapping, and file manipulation.

The user provides the source and destination file names as command-line arguments. The program checks for the existence and accessibility of the source file and prompts the user for confirmation if the destination file already exists. It then maps the source and destination files to memory, copies the content, and unmaps the files from memory. Finally, the program sets the destination file's permissions to match those of the source file and closes both files.

## Features

- Uses memory-mapped I/O
- Maps source and destination into memory
- Source file is protected by read-only permission
- After cloning, storage is unmapped

# File Locator for Environment Variables

This program searches for a specified file within the directories listed in a given environment variable and returns the full path of the found file.

It iterates through each directory in the environment variable, opening and scanning them for the desired file. If the file is found, the program constructs the full path, ensuring that the generated path does not exceed the maximum allowed size. If the file is not found or there is an issue opening a directory, the program displays an error message and continues to the next directory.

The main function requires two command-line arguments: the file name to search for and the environment variable containing the directories.

## Execution

```bash
# Searching for example.txt within the PATH environment
./get-pathname example.txt PATH
```

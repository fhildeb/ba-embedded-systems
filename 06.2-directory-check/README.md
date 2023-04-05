# Directory Detection

This project consists of a small library and a tester program to determine if a given file is a directory. It takes a filename as input and returns `1` if it is a directory and `0` otherwise.

## Static Library

The detection program is using a static library. It contains compiled object code for the directory check function. When the tester program is linked with this library, the object code is included in the final executable, allowing the program to use the check function without requiring the library source code or a dynamic library during runtime.

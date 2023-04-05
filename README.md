# ba-embedded-systems

BA Embedded Systems Course from the University of Applied Science Mittweida that took place in 2019.

> **_NOTE:_** The code commentary might appear in German.

## Course Contents

The course provided a comprehensive understanding of C programming language and included fundamentals like code formation, naming conventions, operators, and data types. Regarding the implementation, the class delved into different function behavior, communication, file and storage management, while also covering various data structures, enumerations, and unions. We also learned about memory management, the different types of pointer handling, and how to work with standard and custom libraries. The course even discussed compilation, preprocessing, and the use of various tools and components.

Regarding the Unix file systems: processes, kernel, signals, and device drivers were explored in-depth, along with its concepts about concurrency, synchronization, and inter-process communication. The communication then continued to cover networks, including sockets and client-server applications.

### Repository Contents

The Repository is split in two main parts:

`templates`

- Collection of **26 sample projects** provided within the course to learn and experiment
- Structured, documented, polished, and set up with builds for easy integration

`main projects`

- Numbered set of **13 own projects** planned and developed from scratch
- Projects have increasing difficulty on changing topics

#### Main Projects

1. Input and Output
2. Data Types and Structures
3. Circle Calculation
4. Bubble Sort Random Inputs
5. File Copy
6. Property Reading
   - File Scanner
   - Directory Check
   - Softlink Detector
7. OS File Operations
   - Memory Mapped Cloning
   - Environment Path Locator
8.
9.
10.
11.
12.
13.

#### Templates

> Look into [templates](./templates/) to get a full description

- `Datagrams`: Connectionless and Settled Communication
- `File Handling`: Write, Read, and Copy
- `Memory Management`: Mapping and Sharing
- `Message Queues`: POSIX and System V
- `Network Transfers`: Sniffing, TCP and UDP Transfers
- `Pipe Handling`: Forking, Duplication, Opening, and Closing
- `Semaphore Restrictions`: Single or Multi-Accesss, Synchronization
- `Signals and Operating Systems`: Delaying, Masking, and Jumping
- `Socket Management`: Pairs and Streams
- `Terminal I/O`: Nonblocks
- `Threads`: Mutex, Data Keys, Syncronization

## Run in Terminal

> Instead of the provided makefiles, you can also run custom tools by compiling the c files. In some cases, further information for dependencies can be found within the makefile.

```bash
#1 Run the makefile inside the folder
make

#2 Run the built executable
./executable [params]
```

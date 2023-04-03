# Reading and Writing using Semaphores

The reader-writer problem is a classic synchronization problem in software engineering that occurs when multiple processes access shared resources. The problem arises when one process is writing to the resource, while others are reading or writing to it concurrently, leading to conflicts and inconsistencies.

## Simple Solution

### Semaphore

The simple semaphore library implements semaphores for interprocess communication and provides functions to initialize, acquire, and release them. Here, a semaphore is acquired and release after a reading or writing process. It is ideal for managing a lightweight memory lock.

> Is used by `simple-reader`and `simple-writer`

### Reader

The reader program is using shared memory to share data between processes. It creates or opens a shared memory segment and reads the data from it using a semaphore to avoid race conditions.

### Writer

The writer program writes a message to the shared memory segment and acquires a write lock using a semaphore to prevent other writers from writing to the segment simultaneously.

## Advanced Solution

### Semaphore

The advanced semaphore implementation has functions for managing a locking mechanism with two different semaphores for reading and writing. The set lock functionality allows for increasing the semaphore range and waits for the other semaphore to be zero. The release lock function decreases the semaphore range.
Within the advanced semaphore solution, waiting times of the programs can be reduced drastically, while also just locking the memory space that is actually needed. This can be utilized if there are multiple processes or threads working on a shared resource concurrently.

### Reader

The reader program reads from a shared memory segment protected by the lock. It acquires the lock for reading, reads the shared memory, releases the lock, and repeats. It uses semaphores to synchronize access to the shared memory between multiple readers.

### Writer

The writer program writes a certain message to a shared memory segment protected by the lock. It acquires the lock for writing, writes to the shared memory, releases the lock, and exits. It uses semaphores to synchronize access to the shared memory between readers and writers.

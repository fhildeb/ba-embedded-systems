# Thread Synchronization with Mutex

## Plain Thread Mutex

This program creates two threads that attempt to access a critical section protected by a mutex. The first thread successfully enters the critical section and exits, while the second thread tries to enter but cancels the first one if it is still in the critical section. The second thread then enters the critical section, sleeps for 4 seconds, and exits.

## Thread Mutex with Cleanup

This program is similar in terms of creating two threads that attempt to access a critical section protected by a mutex. However, it adds thread cleanup functionality to ensure that the mutex is always unlocked, even if a thread is cancelled. The cleanup function unlocks the mutex before the thread exits, preventing potential deadlock or resource leaks.

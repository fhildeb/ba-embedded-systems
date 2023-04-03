#if !defined(__READERWRITER__)
#define __READERWRITER__
int setReadLock(int semid);
int releaseReadLock(int semid);

int setWriteLock(int semid);
int releaseWriteLock(int semid);
int initReadWriteLock(key_t key);
int getIdReadWriteLock(key_t key);
#endif

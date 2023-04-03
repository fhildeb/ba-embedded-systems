#if !defined(__SIMPLESEMA__)
#define __SIMPLESEMA__

int initSema(key_t key, int value);
int P(int semid, int val);
int V(int semid, int val);

#endif

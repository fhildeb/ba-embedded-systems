#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

pthread_key_t key;

void counterinit()
{
  int *cnt = (int *)malloc(sizeof(int));
  *cnt=0;
  pthread_setspecific(key, (void *)cnt);
}

int counter()
{
  int cnt, *cntptr;
  cntptr = (int *)pthread_getspecific(key);
  cnt = ++(*cntptr);
  return cnt;
}


void *thread_fkt(void *p)
{
  char *s = (char *)p;
  int i;
  counterinit();
  for(i=0; i<10; i++) {
    printf("%s: Zähler: %d\n", s, counter());
    sleep(1);
  }
  return NULL;
}

void free_mem(void *cnt)
{
  free(cnt);
}

int main(void)
{
  pthread_t thread1, thread2, thread3;
  int erg;

  pthread_key_create(&key, free_mem);
  erg = pthread_create(&thread1, NULL, thread_fkt, "Thread1");
  assert(erg == 0);
  erg = pthread_create(&thread2, NULL, thread_fkt, "Thread2");
  assert(erg == 0);
  erg = pthread_create(&thread3, NULL, thread_fkt, "Thread3");
  assert(erg == 0);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  return EXIT_SUCCESS;
}

/*  Beispiel aus Wagner / Towsley: "Getting Started With POSIX Threads"
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define pthread_attr_default NULL

void *print_message_function(void *ptr);

int main(void)
{
  pthread_t thread1, thread2;
  char *message1 = "Hello";
  char *message2 = "World\n";

  pthread_create(&thread1, pthread_attr_default, print_message_function,
                 (void *)message1);

  pthread_create(&thread2, pthread_attr_default, print_message_function,
                 (void *)message2);

  sleep(1); /* give threads a little bit time to work */
  return EXIT_SUCCESS;
}

void *print_message_function(void *ptr)
{
  char *message;
  message = (char *)ptr;
  printf(" %s ", message);
  return NULL;
}

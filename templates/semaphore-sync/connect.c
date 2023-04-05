#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

sem_t *semInit(const char *name, int value)
{
  sem_t *sema;
  if ((sema = sem_open(name, O_CREAT | O_EXCL, 0660, value)) == SEM_FAILED)
  {
    if (errno == EEXIST)
    {
      sema = sem_open(name, 0);
    }
    else
      return NULL;
  }
  return sema;
}

int main(int argc, char *argv[])
{
  sem_t *semid;
  if ((semid = semInit("/Connection open", 1)) == NULL)
  {
    perror("sem_open");
    return EXIT_FAILURE;
  }

  printf("Bin ich nun drin oder was ?!?\n");

  if (sem_wait(semid) < 0)
  {
    perror("sem_wait");
    return EXIT_FAILURE;
  }

  printf("Ich bin drin :-)\n");
  getchar();

  if (sem_post(semid) < 0)
  {
    perror("sem_post");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

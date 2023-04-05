/* Kopierprogramm mit Threads
   ist Zieldatei bereits vorhanden, wird abgefragt, ob sie überschrieben
   werden soll
*/

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024
int fdin, fdout;

struct
{
  pthread_mutex_t buffer_mutex; /* sichert exclusiven Zugriff */
  pthread_cond_t buffer_cond;   /* sichert Reihenfolge        */
  int nbytes;                   /* Anzahl gültiger Bytes      */
  int full;                     /* Pufferdaten gültig ?       */
  char buf[BUF_SIZE];
} buffer;

void prerror(int errnum, char *s) /* Gibt Fehler aus,beendet Prozess */
{
  fprintf(stderr, "Fehler : %s %s \n", strerror(errnum), s);
  exit(EXIT_FAILURE);
}

void *erzeuger_thread(void *param)
{ /* liest Datei und schreibt in den Puffer */
  int erg, readd;
  do
  {
    /* Exclusiven Zugriff sichern */
    erg = pthread_mutex_lock(&buffer.buffer_mutex);
    if (erg != 0)
      prerror(erg, "Erzeuger: Mutex Lock");
    /* Warten, bis Puffer leer */
    while (buffer.full == 1)
    {
      erg = pthread_cond_wait(&buffer.buffer_cond, &buffer.buffer_mutex);
      if (erg != 0)
        prerror(erg, "Erzeuger: Cond Wait");
    }
    readd = buffer.nbytes = read(fdin, buffer.buf, BUF_SIZE);
    buffer.full = 1;
    /* bekanntgeben, dass Puffer voll */
    erg = pthread_cond_signal(&buffer.buffer_cond);
    if (erg != 0)
      prerror(erg, "Erzeuger: Cond Signal");
    /* Mutex freigeben, kann auch vor pthread_cond_signal() erfolgen */
    erg = pthread_mutex_unlock(&buffer.buffer_mutex);
    if (erg != 0)
      prerror(erg, "Erzeuger: Mutex Unlock");
  } while (readd > 0);
  return NULL;
}

void *verbraucher_thread(void *param)
{ /* liest Daten aus Puffer, schreibt in Datei */
  int erg, written;
  do
  {
    /* Exclusiven Zugriff sichern */
    erg = pthread_mutex_lock(&buffer.buffer_mutex);
    if (erg != 0)
      prerror(erg, "Erzeuger: Mutex Lock");
    /* Warten, bis Puffer voll */
    while (buffer.full == 0)
    {
      erg = pthread_cond_wait(&buffer.buffer_cond, &buffer.buffer_mutex);
      if (erg != 0)
        prerror(erg, "Verbraucher: Cond Wait");
    }
    written = write(fdout, buffer.buf, buffer.nbytes);
    buffer.full = 0;

    /* Mutex freigeben, kann auch nach pthread_cond_signal() erfolgen */
    erg = pthread_mutex_unlock(&buffer.buffer_mutex);
    if (erg != 0)
      prerror(erg, "Verbraucher: Mutex Unlock");
    /* bekanntgeben, dass Puffer leer */
    erg = pthread_cond_signal(&buffer.buffer_cond);
    if (erg != 0)
      prerror(erg, "Verbraucher: Cond Signal");

  } while (written > 0);
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_t erzeuger, verbraucher;
  char ch;
  int erg;
  void *ergptr;

  if (argc != 3)
  {
    fprintf(stderr, "   usage: copy source dest \n");
    return EXIT_FAILURE;
  }

  if ((fdin = open(argv[1], O_RDONLY)) == -1)
  {
    fprintf(stderr, "   Error! %s doesn't exist! \n", argv[1]);
    return EXIT_FAILURE;
  }

  if ((fdout = open(argv[2], O_CREAT | O_WRONLY | O_EXCL, 0666)) == -1)
  {
    fprintf(stderr, "   %s already exists, overwrite (y/n)?", argv[2]);
    if ((ch = getchar()) == 'y')
    {
      if ((fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
      {
        fprintf(stderr, "   Error! Can't open %s!\n", argv[1]);
        return EXIT_FAILURE;
      }
    }
    else
      return EXIT_FAILURE;
  }

  erg = pthread_mutex_init(&buffer.buffer_mutex, NULL);
  if (erg != 0)
    prerror(erg, "Mutex Init");

  erg = pthread_cond_init(&buffer.buffer_cond, NULL);
  if (erg != 0)
    prerror(erg, "Cond Init");

  erg = pthread_create(&erzeuger, NULL, erzeuger_thread, NULL);
  if (erg != 0)
    prerror(erg, "Erzeuger Init");

  erg = pthread_create(&verbraucher, NULL, verbraucher_thread, NULL);
  if (erg != 0)
    prerror(erg, "Verbraucher Init");

  erg = pthread_join(erzeuger, &ergptr);
  if (erg != 0)
    prerror(erg, "Join Erzeuger");

  erg = pthread_join(verbraucher, &ergptr);
  if (erg != 0)
    prerror(erg, "Join Verbraucher");
  close(fdin);
  close(fdout);
  pthread_cond_destroy(&buffer.buffer_cond);
  pthread_mutex_destroy(&buffer.buffer_mutex);
  return EXIT_SUCCESS;
}

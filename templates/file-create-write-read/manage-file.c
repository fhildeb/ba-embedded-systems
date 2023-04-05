/* legt Datei 'myfile' an, schreibt einen Text hinein und liest ihn aus */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int fd, res;
  char buffer[512];
  struct stat fstat;
  char text[] = "Hallo Welt!\n";

  /* Erzeuge neue Datei, Lösche Inhalt falls vorhanden */
  fd = open("myfile", O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd == -1)
  {
    perror("open: ");
    exit(EXIT_FAILURE);
  }

  /* Schreibe kurzen Text hinein */
  if (write(fd, text, sizeof(text)) != sizeof(text))
  {
    perror("write: ");
    exit(EXIT_FAILURE);
  }

  /* Positioniere auf Dateianfang zurück */
  if (lseek(fd, 0, SEEK_SET) == -1)
  {
    perror("seek: ");
    exit(EXIT_FAILURE);
  }

  /* lies Datei in 100 Byte blöcken ein und gib diese aus,
   * Ende bei Dateiende */
  while ((res = read(fd, buffer, 100)) != 0)
  {
    if (res == -1)
    {
      perror("read: ");
      exit(EXIT_FAILURE);
    }
    buffer[res] = '\0'; /* printf benötigt Stringendekennung */
    printf("%s", buffer);
  }

  /* Datei schliessen */
  if (close(fd) == -1)
  {
    perror("close: ");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

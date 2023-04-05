#include <sys/types.h>
#include <sys/wait.h>
/* 	Lesen und Schreiben aus einer Pipe,
        echte Kommunikation zwischen zwei Prozessen
        Kindprozess
        Daten werden von pread aus pipe gelesen
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  /* wir erben die Leseseite der Pipe */
  int fd, nread;
  char s[100];

  /* den zugehörigen Filedeskriptor erfahren wir über den Parameter */
  fd = atoi(argv[1]);

  printf("reading file deskriptor %d\n", fd);
  nread = read(fd, s, sizeof(s));
  switch (nread)
  {
  case -1:
    perror("read");
    return EXIT_FAILURE;

  case 0:
    fprintf(stderr, "reading EOF or empty pipe\n");
    return EXIT_FAILURE;

  default:
    printf("read %d bytes from pipe: %s \n", nread, s);
  }
  return EXIT_SUCCESS;
}

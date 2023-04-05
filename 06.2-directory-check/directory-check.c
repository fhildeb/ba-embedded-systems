#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Praktikum 4: Aufgabe 2
        Funktion welches auslesen kann, ob die
        angegebene Datei ein Ordner ist
*/
int isDir(char *dateiname)
{
  struct stat information;

  if (stat(dateiname, &information) < 0)
    return 0;

  if (S_ISDIR(information.st_mode))
    return 1;

  return 0;
}

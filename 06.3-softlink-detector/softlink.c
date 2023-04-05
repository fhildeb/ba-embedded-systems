#include <fcntl.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Praktikum 6: Aufgabe 3
        Funktion welche prüft ob Dateiname ein Link ist
*/
char *isSoftLink(char *dateiname)
{
  static char dateiNameVerweis[PATH_MAX];

  // Versuchen, den verlinkten Namen auszulesen
  if (readlink(dateiname, dateiNameVerweis, PATH_MAX) < 0)
    // Bei Misserfolg NULL zurückgeben
    return NULL;
  // Bei Erfolg den verlinkten Dateinamen zurückgeben
  return dateiNameVerweis;
}

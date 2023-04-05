#include "softlink.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Praktikum 6: Aufgabe 3
        Programm wwelches die Funktion IsSoftLink testet
*/
int main(int argc, char *argv[])
{
  // Prüfung, ob eine zu überprüfende Datei mitgegeben wurde
  if (argc != 2 && argc != 3)
  {
    fprintf(stderr, "Programm muss mit folgenden Schemen aufgerufen werden: \n,"
                    "1) ./softlink-tester <Dateiname>,"
                    "2) ./softlink-tester <Dateiname> <Linkerdatei>");
    exit(EXIT_FAILURE);
  }
  // Dateiverlinkung auslesen
  char *analyse = isSoftLink(argv[1]);
  if (analyse == NULL)
  {
    // Datei ist kein Link
    printf("Datei ist kein Link\n");
  }
  else
  {
    // Datei ist ein Link
    printf("Datei ist ein Link\n");
  }
  if (argc == 3)
  {
    analyse = isSoftLink(argv[2]);
    if (analyse == NULL)
    {
      // Datei ist kein Link
      printf("Die angegebenen Linkerdatei ist kein Link auf die erste\n");
    }
    else
    {
      // Datei ist ein Link
      printf("Die angegebenen Linkerdatei ist ein Link auf die erste\n");
    }
  }

  return EXIT_SUCCESS;
}

#include "array.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Praktikum 4: Aufgabe 2
        Programm welches Speicher reserviert, mit
        Zufallszahlen belegt und diese ausgeben kann
*/
int main(int argc, char **argv)
{
  /*Prüfen, ob ein Argument angegeben wurde,
  sonst Abbruch*/
  if (argc <= 1)
  {
    fprintf(
        stderr,
        "Programm wird beendet, es muss genau 1 Parameter angegeben werden\n");
    return EXIT_FAILURE;
  }

  /*Falls Argument vorhanden,
  lies es ein*/

  // Pointer auf Argument
  char *eingabeZeichen = argv[1];

  // In Zahl umwandeln, falls nicht schon geschehen
  int argWert = atoi(eingabeZeichen);

  // Prüfen auf Gültigkeit der Zahl
  if (argWert <= 0)
  {
    fprintf(stderr,
            "Wert des eingegebenen Parameters muss groesser als O sein\n");
    return EXIT_FAILURE;
  }

  // Array mit n Zufallszahlen anlegen und Speicher reservieren
  int *nFeld;
  nFeld = (int *)calloc(argWert, sizeof(int));

  /*Prüfen ob noch genügend Speicher verfügbar ist,
  mit Fehlermeldung beenden*/
  if (nFeld == NULL)
  {
    fprintf(stderr, "Es ist leider nicht genug Speicher verfügbar\n");
    return EXIT_FAILURE;
  }

  // Eintragen von Zufallszahlen in den reservierten Speicher
  for (int i = 0; i < argWert; i++)
  {
    // Funktion rand() gibt Zufallszahl zurück
    int zufallsZahl = rand();
    *(nFeld + i) = zufallsZahl;
  }

  /*Anzeige der im Speicher stehenden Zufallszahlen
  unter Verwendung der geforderten Hilfsfunktion zeigeFeld*/
  fprintf(stdout, "Ungeordnete Zusallszahlen im Speicher: \n");
  zeigeFeld(nFeld, argWert);

  /*Anzeige der geordneten im Speicher steheneden
  Zufallszahlen mit der Hilfsfunktion zeigeSortiertesFeld
  welche auf die externe Methode nFeldSortieren zum Ordnen
  zugreift.

  Es wird eine Hilfsvariable Integer verwendet*/
  fprintf(stdout, "Geordnete Zusallszahlen im Speicher: \n");
  zeigeSortiertesFeld(nFeld, argWert);

  /*Freigeben des Speichers nach der Verwendung,
  um den Speicherraum neu Verwenden zu können*/
  free(nFeld);
  nFeld = NULL;

  // Beenden
  return EXIT_SUCCESS;
}
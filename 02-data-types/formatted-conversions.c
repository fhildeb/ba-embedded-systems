#include <stdio.h>
#include <stdlib.h>

/*
        Praktikum 2: Aufgabe 2
        Eingegebenes Zeichen in verschiedenen Datenformen ausgeben
*/
int main()
{
  // Eingabe
  char input;

  // Solange nicht beendet wird,
  // Eingabe umgewandelt anzeigen
  while (1)
  {
    // Startzeile und Einlesen
    printf("->Drücken sie Eine Taste oder Q zum Beenden: \n");
    scanf(" %c", &input);

    // Falls Beendung gewünscht
    if (input == 'Q')
    {
      return EXIT_SUCCESS;
    }
    else
    {
      // Ausgabe in verschiedenen Datentypen
      // inklusive Formatierung
      //%c Zeichen
      //%u unsigned
      //%x Hexadezimal
      printf("Anzeige: \n     '%c'\n     %03u\n    %#4x \n", input, input,
             input);
    }
  }
}

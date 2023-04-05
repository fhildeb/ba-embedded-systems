#include <stdio.h>
#include <stdlib.h>

/*
	Praktikum 2: Aufgabe 1
	Zeichen in verschiedenen Datentypen ausgeben
*/
int main()
{
	//Eingabe
	char input;
	
	//Solange nicht beendet wird, 
	//Eingabe umgewandelt anzeigen
   while(1) 
   {
   	
   	//Startzeile und Einlesen
      printf("->Druecken sie Eine Taste oder Q zum Beenden: \n");
      scanf(" %c",&input);

	  //Falls Beenden gewünscht
      if(input == 'Q')
      {
          return EXIT_SUCCESS;
      }
      else
      {
          //Ausgabe in verschiedenen Datentypen
          //%c Zeichen
          //%u unsigned
          //%x Hexadezimal
	      printf("Anzeige: '%c' %d %x \n", input, input, input);
      }	  
    }
}                    

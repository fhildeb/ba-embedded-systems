//Einbeziehen von Headerdateien
#include <stdio.h>
#include <stdlib.h>

/*
	Praktikum 1: Aufgabe 3
	Ausgabe der Char-Anzahl eines Strings
	@author: fhildeb1
*/
int main()
{

    //Variable zum Speichern der Anzahl
    //"return value"
	int retval = 0;

	//Anzahl definieren
	//Leerzeichen und Zeilenumbruch wird mitgezählt
	retval = printf("Das ist ein Beispieltext\n");

	//Anzahl der verwendeten Strings samt
	//Zeilenumbruchs-Zeichen ausgeben
	printf("retval = %d\n", retval);

	//Beenden
	return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "flaeche.h"

/* Praktikum 3: Aufgabe 2-6
	Definition der Funktionen welche für
	kreis.c benötigt werden
*/

/* Definition der Funktion kreisFlaeche() 
	Rückgabewert = berechnete Kreisfläche*/
double kreisFlaeche(double radius)
{
    static int zaehler = 0;
	 double value = Pi();
	 
	 /* Zusatzaufgabe 6: Ausgabe einer Warnmeldung
	 	nach dem 5. Aufruf der Funktion*/
	 	
	 ++zaehler;
	 
	 if(zaehler > 4)
	 {		 
		 printf("Warnung: kreisFlaeche() wurde %d Mal aufgerufen\n", zaehler);
	 }    
    return value*radius*radius;
}

/* Definition der Funktion Pi() */
double Pi()
{
	/*Mit ASIN-Funktion*/
	return (double)asin(+1)*2;	
	
	
	/*Ohne ASIN-Funktion*/
	/*
		double piA,piB;
		int n;

		piA = 2 * sqrt(3);
		piB = 3;

		for (n = 0; n <= 25; n ++)
		{
			piA = 2 * piA * piB / (piA + piB);
			piB = sqrt (piA * piB);

			return piB;
		}
		return 0;
	*/
}
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "array.h"

/* Praktikum 4: Aufgabe 2
	Definition der Funktionen welche zum
	sortieren und zur Ausgabe von Zeichen im
	reservierten Speicher dienen
*/
	/*Anzeige des übergebenen Feldes in der Kommandozentrale*/
	void zeigeFeld(int* inputFeld, int inputGroesse )
	{
		for(int i = 0; i < inputGroesse; i++){
			fprintf(stdout, "Inhalt Feld [%d]: %d\n", i, *(inputFeld+i));	
		}
	}
	
	/*Sortierte Anzeige des übergebenen Feldes in der 
	Kommandozentrale*/
	int zeigeSortiertesFeld(int* inputFeld, int inputGroesse)
	{	
	
		//Speicher für den Zeigerarray reservieren
		int ** feldZeiger = (int**) calloc(inputGroesse, sizeof(int*));
		
		//Prüfen ob noch genug Speicher für Reservierung verfügbar ist
		if(feldZeiger == NULL)
		{
			fprintf(stderr, "Nicht mehr genug Speicher verfügbar");
			return EXIT_FAILURE;		
		}
		
			
		//Zeigerarray initialisieren
		for(int i = 0; i < inputGroesse; i++) {
			feldZeiger[i] = &inputFeld[i];
		}
		
		sortWithBubbleSort(feldZeiger, inputGroesse);			
		
		/*Ausgabe der Zahlen */
		for(int i = 0; i < inputGroesse; i++)
		{
			printf("Inhalt Feld [%d]: %d\n",i, *(feldZeiger[i]));		
		}
		
		//Speicher freigeben für weitere Benutzung
		free(feldZeiger);
		feldZeiger = NULL;
		
		//Beenden
		return EXIT_SUCCESS;
	}
	
	void sortWithBubbleSort(int** zeiger, int uebergabeFeldGroesse)
	{
		//Sortieren mit dem BubbleSortAlgorithmus
		for(int i = 0; i < uebergabeFeldGroesse; i++) 
		{
			for(int j = i+1; j < uebergabeFeldGroesse; j++) 
			{
				if (*(zeiger[j]) < *(zeiger[i])) 
				{
				/*BubbleSort nutzt eine temporäre
				Variable zum Zwischenspeichern eines Feldinhalts*/
					int* tempWert = NULL;
					tempWert = zeiger[i];
					zeiger[i] = zeiger[j];
					zeiger[j] = tempWert;
				}
			}
		}
	}
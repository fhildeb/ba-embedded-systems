#include <stdio.h>
#include <stdlib.h>

/*
	Praktikum 2: Aufgabe 3
	Größe der Datentypen ausgeben
	@author: fhildeb1
*/
int main()
{
       //Benutzen von sizeof() zur Entnahme der Größe
       printf("Die Datentypen haben folgende Adressgröße: \n(Eine Adresse entspricht einem Byte)\n)");
	   printf("Der Typ char   belegt %03d Adressen\n",(int) sizeof(char));
	   printf("Der Typ int    belegt %03d Adressen\n",(int) sizeof(int));
	   printf("Der Typ short  belegt %03d Adressen\n",(int) sizeof(short));
	   printf("Der Typ long   belegt %03d Adressen\n",(int) sizeof(long));
	   printf("Der Typ float  belegt %03d Adressen\n",(int) sizeof(float));
	   printf("Der Typ double belegt %03d Adressen\n",(int) sizeof(double));

	   return EXIT_SUCCESS;
}                           
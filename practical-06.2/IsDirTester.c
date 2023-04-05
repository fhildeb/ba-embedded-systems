#include <stdio.h>
#include <stdlib.h>
#include "IsDir.h"

/* Praktikum 6: Aufgabe 2
	Programm welches die Funktion IsDir testet
*/
int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		fprintf(stderr,"Das Programm ist mit folgendem Schema aufzurufen:,\n"
		"./IsDirTester <Dateiname>\n");
		exit(EXIT_FAILURE);
	}
	
	int input = isDir(argv[1]);
	if (input) 
		printf("%s ist ein Ordner", argv[1]); 
	else
		printf("%s ist kein Ordner",argv[1]);
		
	return EXIT_SUCCESS;	
}

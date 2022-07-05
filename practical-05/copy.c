//Ein-/Ausgabefunktionen
#include <stdio.h>

//Umwandlung Zahlen-/Speicherausdrücke
#include <stdlib.h>

//Wird für Datei-Funktionen benötigt
#include <unistd.h>
//Dateibehandlung
#include <fcntl.h>

//Definiert zurückgegebene Struktur der Daten
#include <sys/stat.h>

//Definieren der Puffergröße mit welcher gearbeitet wird
#define BUFFER_SIZE 1024

/* Praktikum 5:
	Programm zum kopieren von Datei <param1> nach <param2>
	mit identischem Inhalt und Rechten
	@author: fhildeb1
*/

int main(int argc, char* argv[])
{
	//Genutzte Variablen:
	
	//Übergebene Parameter
	char* quelldatei = argv[1];
	char* zieldatei = argv[2];
	
	/*File Descriptor als Handler bezüglich der Dateien*/
	int file_descriptor_quelldatei; 
	int file_descriptor_zieldatei;
	
	struct stat quelle_eigenschaften;
	
	
	/* Falls Argumente nicht dem gewünschten 
	Schema entsprechen */
	if (argc != 3)
	{
		fprintf(stderr, "Das Programm soll in folgendem Schema,"
		"aufgerufen werden: \n./copy <Quelldatei> <Zieldatei>\n");
		fprintf(stderr, "VORSICHT: Das Programm ersetzt ggf. Dateien"
		"mit dem Inhalt der Quelldatei!\n");	
		exit(EXIT_FAILURE);	
	}
	
	/* Überprüfen, ob die Quelldatei 
	wirklich existiert bzw. Leserechte hat */
	file_descriptor_quelldatei = open(quelldatei, O_RDONLY);
	if (file_descriptor_quelldatei == -1) 
	//Falls Datei/Leserechte nicht vorhanden
	{
		perror("Fehler beim öffnen der Quelldatei ");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("--Status: Quelldatei gefunden und Rechte vorhanden\n");
	}
	
	/*Überprüfen des Zugriffsrechtes auf die Datei*/
	if( access(zieldatei, F_OK) == 0)
   {
		printf("--Status: Es wird auf Zieldatei zugegriffen\n");	 
		  
   }
   else 
   {
		printf("--Status: Zieldatei wird im nächsten Schritt erstellt\n");
   }
   
	/*Zieldatei öffnen und Inhalt löschen
	mit Paramerter für: Nur_Schreiben+Ersetzen+Neu_Erstellen*/
	file_descriptor_zieldatei = open(zieldatei, O_WRONLY | O_TRUNC | O_CREAT);
   if(file_descriptor_zieldatei == -1)
   //Falls Datei nicht beschreibbar
   {
      perror("Fehler beim öffnen der Zieldatei");
      exit(EXIT_FAILURE);
   }

   /*Überprüfen ob Quelldateieigenschaften erfasst werden können*/
   if( fstat(file_descriptor_quelldatei, &quelle_eigenschaften) == -1 )
   {
      perror("Fehler beim entnehmen der Eigenschaften der Quelldatei");
      exit(EXIT_FAILURE);
   }

   /*Überprüfen ob Rechte der Zieldatei auf die der Quelldatei 
   geändert werden können*/
   if( fchmod(file_descriptor_zieldatei, quelle_eigenschaften.st_mode) == -1)
   {
		perror("Fehler beim ändern der Rechte der Zieldatei");
      exit(EXIT_FAILURE);
   }
	
	/*Deklarieren von Variablen für das Kopieren*/
	int geleseneBytes, geschriebeneBytes;
	char kopierbuffer[BUFFER_SIZE];
	   
   /*Kopieren der Datei*/
   printf("--Status: Kopiervorgang startet\n");
   while((geleseneBytes = read(file_descriptor_quelldatei, 
   							       &kopierbuffer, BUFFER_SIZE)) >0)
   {
		geschriebeneBytes = write(file_descriptor_zieldatei, 
										 &kopierbuffer, geleseneBytes);
		if(geschriebeneBytes == -1)
		{
			perror("Fehler beim schreiben des Inhaltes in die Zieldatei");
			exit(EXIT_FAILURE);		
		}	 	   
   }
   printf("--Status: %d Bytes geschrieben\n", geschriebeneBytes);
   printf("--Status: Kopiervorgang beendet\n");
   
   /*Dateien nach dem Kopiervorgang schließen*/
   close(file_descriptor_quelldatei);
   close(file_descriptor_zieldatei);
   
   return EXIT_SUCCESS; 
	
}





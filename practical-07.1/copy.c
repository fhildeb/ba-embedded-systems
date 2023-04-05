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

//Memory-Map / Mamory-Unmap
#include <sys/mman.h>

// Memory-Copy
#include <string.h> 

/* Praktikum 7:
	Programm zum kopieren von Datei <param1> nach <param2>
	mit identischem Inhalt und Rechten
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
	
	//Länge der Datei
	size_t dateilaenge;

	//Addressen für MMAP
   void* quelle_addresse = NULL;
   void* ziel_addresse = NULL;
	
	
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
	file_descriptor_quelldatei = open(quelldatei, O_RDWR | O_CREAT);
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
		 char in;
        printf("Wollen sie die Datei Ueberschreiben? (j/n)");
        scanf(" %c", &in);
        while(getchar()!='\n')
            ;

        if( in != 'j')
        {
            exit(EXIT_FAILURE);
        }
		  
   }
   else 
   {
		printf("--Status: Zieldatei wird im nächsten Schritt erstellt\n");
   }
   
	/*Zieldatei öffnen und Inhalt löschen
	mit Paramerter für: Nur_Schreiben+Ersetzen+Neu_Erstellen*/
	file_descriptor_zieldatei = open(zieldatei, O_RDWR | O_TRUNC | O_CREAT);
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
	
   /*Kopieren der Datei*/
   printf("--Status: Kopiervorgang startet\n");

	/* länge der zu kopierenden Datei finden */
   dateilaenge = quelle_eigenschaften.st_size;
   
    /*Beschneiden der Zieldatei auf Länge der Quelldatei*/
    if( ftruncate(file_descriptor_zieldatei, dateilaenge) < 0)
    {
        perror("FTRUNC");
        exit(EXIT_FAILURE);
    }
   
	/* Dateien in den Speicher mappen */   
     
   
   quelle_addresse = mmap(0, dateilaenge, PROT_READ , MAP_PRIVATE,
   							 file_descriptor_quelldatei, 0);

   if(quelle_addresse == MAP_FAILED)
   {
       perror("Fehler beim mappen der Quelldatei: ");
       exit(EXIT_FAILURE);
   }
    
 	/*Parameter: Adresse, Länge, Sicherung, Flags, Filedescriptor, Offset*/
   ziel_addresse = mmap(0, dateilaenge, PROT_READ | PROT_WRITE,
   				 MAP_PRIVATE, file_descriptor_zieldatei, 0);

   if(ziel_addresse == MAP_FAILED)
   {
       perror("Fehler beim mappen der Zieldatei: ");
       exit(EXIT_FAILURE);
   }   
   
   /* Speicher kopieren */
   memcpy( ziel_addresse, quelle_addresse, dateilaenge );
	
   /* Dateien aus dem Speicher nehmen */
   munmap(ziel_addresse, dateilaenge);
   munmap(quelle_addresse, dateilaenge);

   printf("--Status: Kopiervorgang beendet\n");
   
   /*Dateien nach dem Kopiervorgang schließen*/
   close(file_descriptor_quelldatei);
   close(file_descriptor_zieldatei);
   
   return EXIT_SUCCESS; 
	
}





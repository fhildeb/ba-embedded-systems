#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>


/* Praktikum 6 Aufgabe 1:
	Programm zum anzeigen der Dateieigenschaften
	der als <param1> übergebenen Datei
	@author: fhildeb1
*/

/*Prototyp der showStat-Funktion*/
void showStats(char* input_name);
int wandleDateitypInString(mode_t mode);
const char * datenTypen[] = {"regular file","directory","character device","block device", "FIFO","symbolic link","socket"};

int main(int argc, char * argv[])
{
	char* datei = argv[1];
	
	//Prüfen ob der Parameter angegeben wurde
	if(argc != 2)
	{
	fprintf(stderr,"Programm wurde nicht mit richtigem Schema aufgerufen \nBenutzen Sie: ./showStat <Dateiname>\n");
	exit(EXIT_FAILURE);	
	}
	
		/*Dateityp gibt bei regulärer Aufrufsweise einen Lesefehler
	zurück und bricht Programm ab. Deswegen Behandlung aufrufen
	und in String umwandeln*/
		
	int wandleDateitypInString(mode_t mode)
	{
		if (S_ISREG(mode) != 0 ) 
		{
			return 0;
		} 
		else if(S_ISDIR(mode) != 0 ) 
		{
			return 1;
		} 
		else if(S_ISCHR(mode) != 0 ) 
		{
			return 2;
		} 
		else if(S_ISBLK(mode) != 0 ) 
		{
			return 3;
		} 
		else if(S_ISFIFO(mode) != 0 ) 
		{
			return 4;
		} 
		else if(S_ISLNK(mode) != 0 ) 
		{
			return 5;
		} 
		else 
		{
			return 6;
		}
	}
	
	/*Funktion zum Lesen der Dateieigenschaften und
	Ausgabe des:
	
	-NAMEN		-Gerätenummer		-i-Node-Nummer		-Linkzähler
	-Dateityp	-Zugriffsrechte	-Besitzer			-Gruppe
	-Dateigröße	-Zeiteinträge	
	*/
	void showStats(char* input_name)
	{
		/*File Descriptor als Status bezüglich der Dateien*/
		int file_descriptor;
	
		/*Zeitangeben*/
		time_t accesstime, lastchangetime, lastmodificationtime;
	
		/*Dateieigenschaften*/
		struct stat eigenschaften;
	
		/*Benutzer-Account Informationen*/
		struct passwd* benutzerInformation;
	
		/*Gruppeninformationen*/
		struct group* gruppenInformation;
		
		//Datei lesen
		file_descriptor = open(input_name, O_RDONLY);
		if(file_descriptor == -1)
		//Falls Datei nicht gelesen werden konnte
		{
			perror("Fehler beim Lesen der Datei: ");
			exit(EXIT_FAILURE);	
		}
	
		//Dateiinformationen auslesen
		if(fstat(file_descriptor, &eigenschaften) == -1)
		{
			perror("Fehler beim Lesen der Eigenschaften: ");
			close(file_descriptor);
			exit(EXIT_FAILURE);	
		}
	
		//Benutzerinformationen auslesen
		benutzerInformation = getpwuid(eigenschaften.st_uid);
		if(benutzerInformation == NULL)
		{
			perror("Fehler beim Lesen der Benutzereigenschaften: ");
			close(file_descriptor);
			exit(EXIT_FAILURE);	
		}
	
		//Gruppeninformationen auslesen
		gruppenInformation = getgrgid(eigenschaften.st_gid);	
		if(gruppenInformation == NULL) 
		{
			perror("Fehler beim Lesen der Gruppeneigenschaften: ");
			close(file_descriptor);
			exit(EXIT_FAILURE);
		}
	
		accesstime = eigenschaften.st_atime;
		lastmodificationtime = eigenschaften.st_mtime;
		lastchangetime = eigenschaften.st_ctime;	
	
		/*Dateiinformationen ausgeben*/
		printf("_______________________Name: %s\n",input_name);
		printf("_______________Gerätenummer: %ld\n",eigenschaften.st_dev);
		printf("______________i-Node-Nummer: %ld\n",eigenschaften.st_ino);
		printf("_________________Linkzähler: %ld\n",eigenschaften.st_nlink);
		printf("___________________Datentyp: %s\n",datenTypen[wandleDateitypInString(eigenschaften.st_mode)]);
		printf("_____________Zugriffsrechte: %o\t\n",eigenschaften.st_mode);
		printf("______Benutzerinformationen: %s: %d\n",benutzerInformation->pw_name,eigenschaften.st_uid);
		printf("_______Gruppeninformationen: %s: %d\n",gruppenInformation->gr_name,eigenschaften.st_gid);
		printf("_________________Dateigröße: %ld\n",eigenschaften.st_size);
		printf("________________Erstellt_am: %.19s\n",ctime(&accesstime));
		printf("_____Als_letztes_benutzt_am: %.19s\n",ctime(&lastmodificationtime));
		printf("____Als_letztes_geändert_am: %.19s\n",ctime(&lastchangetime));
		close(file_descriptor);
	}
	
	/*Datei kann entweder Regulär oder Verzeichnis sein:
	Deswegen Überprüfung/Behandlung dieser zwei Möglichkeiten*/
	
	//Auf Verzeichnis prüfen
	DIR* verzeichnis = opendir(datei);
	if(verzeichnis == NULL)
	//Datei ist kein Verzeichnis
	{
		printf("Datei ist einzelne Datei mit folgenden Eigenschaften:\n");		
		showStats(datei);
		return EXIT_SUCCESS;	
	}
	else 
	//Datei ist Verzeichnis
	{
		struct dirent* beinhalteteDateien;
		printf("Datei ist Verzeichnis mit folgenden Dateien und dessen Eigenschaften:\n");		
		/*Solange das Ende des Verzeichnisses nicht erreicht
		wurde, Dateieigenschaften weiter ausgeben*/
		while( (beinhalteteDateien = readdir(verzeichnis)) != NULL)
		{
			printf("\nStart-of-File---------------------------\n");
			showStats(beinhalteteDateien->d_name);
			printf("End-of-File-----------------------------\n");
		}
		return EXIT_SUCCESS;
	}
	
	
		

	
}
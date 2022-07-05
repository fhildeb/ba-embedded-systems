#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/param.h>
#include <fcntl.h> 
#include <linux/limits.h>
#include <string.h>

/* Praktikum 6: Aufgabe 3
	Funktion welche prüft ob Dateiname ein Link ist
	@author: fhildeb1
*/
char* isSoftLink(char* dateiname) {
	static char dateiNameVerweis[PATH_MAX];
	
	//Versuchen, den verlinkten Namen auszulesen
	if (readlink(dateiname,dateiNameVerweis,PATH_MAX) < 0)
		//Bei Misserfolg NULL zurückgeben
		return NULL;
	//Bei Erfolg den verlinkten Dateinamen zurückgeben
	return dateiNameVerweis;
}

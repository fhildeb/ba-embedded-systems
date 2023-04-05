#include <stdlib.h> /* getnenv */
#include <linux/limits.h> /* PATH_MAX */
#include <dirent.h> /* verzeichnisse lesen */
#include <string.h> /* strlen, strcpy, strcat */
#include <stdio.h> /* perror */

/* Praktikum 7:
	Programm gibt den vollen Pfadnamen einer gesuchten Datei zurück,
	welche in der angegebenen Umgebungsvariable eingetragen sein soll.

*/

/*Prototyp zum Anzeigen des vollständigen Pfads der Datei welche
in den Umgebungsvariablen gesucht wird*/
char* GetFullPathName(char* datei, char* Umgebungsvariable);

/*Hilfsfunktion um durch die Umgebungsvariablen zu iternieren die
mit Doppelpunkt getrennt werden*/
static char* UmgebungsvariableIterieren(char* uVariable)
{
   static long offset = 0;
		
   static char buffer[PATH_MAX];

   if( offset >= strlen(uVariable) )
   {
       /* liste zu Ende */
       return NULL;
   }

   int i = 0;
   while( uVariable[i+offset] != ':' && uVariable[i+offset] != '\0' )
   {
       buffer[i] = uVariable[i+offset];
       i++;
   }

   buffer[i] = '\0'; /* String ist zuende*/
   ++i; /* Doppelpunkt überspringen*/
   offset += i;

   return buffer;
}

char* GetFullPathName(char* name, char* ENVname)
{
   char* ENVval;
   char* path;
   static char fullpath[PATH_MAX];
   DIR* verzeichnis;
   struct dirent* eintrag;
   int found = 0;

   /* Systemvariable holen */
   ENVval = getenv(ENVname);
   if( ENVval == NULL )
   {
       return NULL;
   }

   /*Durch alle Systemvariablen wechseln*/
   while( (path = UmgebungsvariableIterieren(ENVval)) != NULL )
   {
       /*Verzeichnis öffnen*/
       verzeichnis = opendir(path);
       if(verzeichnis == NULL)
       {
           fprintf(stderr, "Konnte Verzeichnis %s nicht oeffnen: ", path);
           perror("");
           continue; /*Nächstes Verzeichnis zu öffnen*/
       }

       /*Über alle Einträge iterieren*/
       while( (eintrag = readdir(verzeichnis)) != NULL )
       {
           if(strcmp(name, eintrag->d_name) == 0)
           {
               /* gefunden, verlasse eintrag-loop */
               found = 1;
               break;
           }
       }

       /*Falls gefunden, verlasse Verzeichnis*/
       if(found == 1)
       {
           size_t len = strlen(path);

           /*Problem falls größer als unser Rückgabe-Array*/
           if(len >= PATH_MAX-1)
           {
               return NULL;
           }

           /*len+1, da das \0 auch kopieren wollen*/
           strncpy(fullpath, path, len+1);

           /* "/" an Pfad anhängen, falls nicht auf "/" endet */
           if( fullpath[len-1] != '/' )
           {
               fullpath[len] = '/';
               fullpath[len+1] = '\0';
           }

           /* problem falls nicht genug platz */
           size_t len2 = strlen(name);
           if( len+len2 >= PATH_MAX-1 )
           {
               return NULL;
           }

           strncat(fullpath, name, len2+1);
           return fullpath;
       }
   }

    return NULL; /* sonst */
}

int main(int argc, char* argv[])
{
   if(argc != 3)
   {
       printf("Programm muss in folgendem Schema aufgerufen werden: \n");
       printf("./get-pathname <Dateiname> <Umgebungsvariable>\n");
       return EXIT_FAILURE;
   }

   char* pfad;
   
   pfad = GetFullPathName(argv[1], argv[2]);

   if(pfad == NULL)
   {
       printf("Fehler, Datei konnte nicht gefunden werden!\n");
       return EXIT_FAILURE;
   }

   printf("Datei gefunden in: %s\n", pfad);

   return EXIT_SUCCESS;
}
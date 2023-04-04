#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define LEN  120

int main()
{
    int res;
    char *map;

    int filed = open("/tmp/mmap.dat", O_CREAT|O_RDWR|O_TRUNC, 0600);
    if (filed < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    // Auf vorletztes Byte positionieren (Zählung beginnt mit 0)
    if ((res = lseek(filed, LEN-2, SEEK_SET)) < 0) {
        perror("lseek");
        return EXIT_FAILURE;
    }

    // NUL (Endezeichen einer Zeichenkette) schreiben
    if ((res = write(filed, "", 1)) < 0) {
        perror("write");
        return EXIT_FAILURE;
    }

    /* alternativ:  
    if((res = ftruncate(filed, LEN)) < 0) {
        perror("truncate");
        return EXIT_FAILURE;
    }
    */
    
    // Datei in Speicher mappen, 
    if ((map = mmap(0, LEN, PROT_READ|PROT_WRITE, 
                MAP_SHARED, filed, 0)) == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // etwas in SHM-Bereich schreiben
    strncpy(map, "Hallo ! \n", LEN); 

    // auf "Enter-Taste" warten, anderer Prozess muss Botschaft abholen
    getchar();
    
    // Mapping aufheben
    if (munmap(map, LEN) < 0) {
        perror("munmap");
        return EXIT_FAILURE;
    }
    
    // Datei schließen
    if (close(filed)) {
        perror("close");
        return EXIT_FAILURE;
    }
    
    // Temporäre Datei löschen
    if ((res=unlink("/tmp/mmap.dat")) < 0) {
        perror("close");
            return EXIT_FAILURE;
    }
                                            
    return EXIT_SUCCESS;
}

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
    char *map;

    int filed = open("/tmp/mmap.dat", O_RDONLY);

    if (filed < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    // Datei in Speicher mappen
    if ((map = mmap(0, LEN, PROT_READ, 
              MAP_SHARED, filed, 0)) == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // Inhalt der gemappten Datei als Zeichenkette ausgeben
    puts(map); 

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
    
    return EXIT_SUCCESS;
}

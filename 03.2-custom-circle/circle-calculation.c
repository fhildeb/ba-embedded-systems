/* ----------------------- Berechnungen am Kreis ------------------------ */

/* Einfügen benötigter Headerdateien in Quelltext   */
/* - enthalten Prototypen von Bibliotheksfunktionen */
/*   wie printf(), scanf()                          */

#include <stdio.h>
#include <stdlib.h>
#include "area.h"

/* Definition der Funktion main()                   */
int main(int argc, char *argv[])
{
    /* Definition lokaler Variablen                 */
    double r, u, flaeche;
    char jn;

    int erg;
     
    /* Versuche zählen */
    static int versuch = 0;
    
    /* Anweisungsteil */
    do {
        printf("Geben Sie den Radius ein : ");
        erg=scanf("%lg",&r);     // erg sollte ueberprueft werden!
        
			/*Versuche erhöhen */        
        ++versuch;
        
        /* Konstante ersetzt durch Pi() */
        u = 2*Pi()*r;
        flaeche = kreisFlaeche(r);

        /* Versuche ausgeben */ 
        printf("Umfang: %lg  Flaeche: %lg Versuch: %d \n", u, flaeche, versuch);

        printf(" Nochmal (j/n)?");
        erg = scanf(" %c", &jn);

    } while(jn == 'j');
    return EXIT_SUCCESS;     /* EXIT_SUCCESS (0) alles ok */
}                           

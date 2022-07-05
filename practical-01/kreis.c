/* ----------------------- Berechnungen am Kreis ------------------------ */

/* Definition der Konstanten Pi durch Präprozessoranweisung #define       */
/* jedes PI wird im Text durch 3.1415926 ersetzt                          */

#define PI 3.1415926

/* Einfügen benötigter Headerdateien in Quelltext   */
/* - enthalten Prototypen von Bibliotheksfunktionen */
/*   wie printf(), scanf()                          */

#include <stdio.h>
#include <stdlib.h>

/* Definition der Funktion kreisFlaeche() */
double kreisFlaeche(double radius)
{
    return PI*radius*radius;
}

/* Definition der Funktion main()                   */
int main(int argc, char *argv[])
{
    /* Definition lokaler Variablen                 */
    double r, u, flaeche;
    char jn;

    int erg; 
    
    /*edit @author: fhildeb1
    Versuche zählen */
    static int versuch = 0;
    
    /* Anweisungsteil */
    do {
        printf("Geben Sie den Radius ein : ");
        erg=scanf("%lg",&r);     // erg sollte ueberprueft werden!
        
        /*edit @author: fhildeb1
    	  Versuche erhöhen */
        ++versuch;
        
        u = 2*PI*r;
        flaeche = kreisFlaeche(r);

        /*edit @author: fhildeb1
    	  Versuche ausgeben */
        printf("Umfang: %lg  Flaeche: %lg Versuch: %d \n", u, flaeche, versuch);

        printf(" Nochmal (j/n)?");
        erg = scanf(" %c", &jn);

    } while(jn == 'j');
    return EXIT_SUCCESS;     /* EXIT_SUCCESS (0) alles ok */
}                           

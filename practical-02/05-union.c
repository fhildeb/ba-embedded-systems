#include <stdio.h>
#include <stdlib.h>

/*
	Praktikum 2: Aufgabe 4b
	Strucktur in Union anlegen
	@author: fhildeb1
*/
//Strucktur innerhalb des Union definieren
union testunion{
    struct simplestruct {
        char c;
        int x;
    }teststruct;
    unsigned char txt[8];
};

int main(int argc, char *argv[])
{

    //Komponente von Typ testunion anlegen
    union testunion component;

    //Infotext
    printf("INFO: Durch das Anlegen eines Unions wird Speicherplatz gespart, indem man\n"
           "      Datentypen 'ueberlappen' laesst dh. eine Art Speicherreservierung fuer\n"
           "      unterschiedliche Datentypen taetigt. Die Groesse ist dabei nur so maechtig wie\n"
           "      der groesste darin vorkommende Datentyp (In diesem Fall Int). \n"
           "      Es wird immer nur ein Datentyp gespeichert und wenn noetig, ueberschrieben\n\n"
           "Groesse des Structs:      %d Byte (Bestehend aus 1x Char und 1x Int)\n"
           "Groesse unsigned Chars:   %d Byte (Bestehend aus 8x Char)\n"
           "Groesse des Union:        %d Byte\n\n",(int) sizeof(component.teststruct),(int) sizeof(component.txt),(int) sizeof(component));

    //Union für Chars füllen
    component.txt[0] = 'T';
    component.txt[1] = 'e';
    component.txt[2] = 's';
    component.txt[3] = 't';
    component.txt[4] = 'T';
    component.txt[5] = 'e';
    component.txt[6] = 'x';
    component.txt[7] = 't';
    printf("-->Unsigned Chars wurden beschrieben\n");

    //Union auswerten
    for(int i = 0; i < (int) sizeof(component.txt); i++){
        printf(" Inhalt Union Byte[%d] (Hexadezimal): %x (Zeichen) %c\n", i, component.txt[i], component.txt[i]);
    }

    //Union mit Struct füllen
    component.teststruct.c = 'z';
    component.teststruct.x = 1000000;
    printf("\n-->Struktur beschrieben \n"
           "(Byte 1-3 wird nicht mit ueberschrieben, da Speicherbereich im Struct leer. (Siehe 4a)\n");
        printf("Inhalt teststruct.c: %c bzw. %x \n", component.teststruct.c, component.teststruct.c);
        printf("Inhalt teststruct.x: %d bzw. %x  \n", component.teststruct.x, component.teststruct.x);

    //Auswertung Union
    for(int i = 0; i < (int) sizeof(component.txt); i++){
        printf("Inhalt Union Byte[%d] (Hexadezimal): %x (Zeichen) %c\n", i, component.txt[i], component.txt[i]);
    }

    //Beenden
    return EXIT_SUCCESS;
}
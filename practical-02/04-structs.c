#include <stdio.h>
#include <stdlib.h>

/*
	Praktikum 2: Aufgabe 4
	Erlaeutern des Aufbaus einer Strucktur
	@author: fhildeb1
*/
//Strucktur definieren
struct test
{
    char c;
    int x;
};

int main()
{
    //Variable der Strucktur test deklarieren
    struct test component;

    //Werte zuweisen
    component.c = 'z';
    component.x = 1000000;

    printf("INFO:   Die Struktur setzt sich aus einem CHAR (%d Byte) "
           "und einem INT (%d Byte) zusammen. \n",
            (int) sizeof(char),
            (int) sizeof(int));
    printf("        Die Datenstruktur speichert die Datentypen in 4 Byte Adressen ab. Unterschiedliche \n"
           "        aufeinanderfolgende Datentypen beginnen jedoch mit neunen Byte. Fuellt der vorherige \n"
           "        Datentyp den Platz von 4 Byte nicht aus, so bleibt dieser Speicher frei.\n");
    printf("        Die Variable 'component' der Strucktur 'test' belegt somit insgesamt %d Byte. \n"
           "        anstatt der lediglich benoetigten 5 Byte. \n\n",(int) sizeof(component));
    printf("SKIZZE:     Startdresse       Inhalt (4 Byte)     \n"
           "        |     ...                 ...          | \n"
           "        |--------------|-----------------------| \n"
           "        |%p|        1000000        | \n"
           "        |--------------|-----------------------| \n"
           "        |%p| 'z' |     |     |     | --> 3 Freie Bytes \n"
           "        |--------------|-----------------------| \n"
           "        |     ...      |          ...          | \n"
           "        |--------------|-----------------------| \n"
           "        |00000000000000| * * * * * * * * * * * | \n"
           "        |- ------------|-----------------------| \n"
           ,(void*)&component.c,(void*)&component.x );
    return EXIT_SUCCESS;
}
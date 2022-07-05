#Option zum erstellen mit
#Debug-Informationen für GDB
#Zeile 4 auskommentieren:
//DEBUG = -g

#Option zum Löschen von Objekt-
#und AutoSave-Dateien: 
#: make clean
.PHONY: clean

#Binary erstellen:
#: make
nFeldZufall:	nFeldZufall.o	nFeldFunktionen.o
	gcc $(DEBUG) -Wall -o nFeldZufall nFeldZufall.o nFeldFunktionen.o -lm -std=c11

nFeldZufall.o:	nFeldZufall.c
	gcc $(DEBUG) -Wall -c nFeldZufall.c -std=c11

nFeldFunktionen.o:	nFeldFunktionen.c
	gcc $(DEBUG) -Wall -c nFeldFunktionen.c -lm -std=c11
	
clean:
	rm -f *.o
	rm -f *~
	
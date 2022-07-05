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
kreis:	flaeche.o	kreis.o
	gcc $(DEBUG) -Wall -o kreis kreis.o flaeche.o -lm -std=c11

kreis.o:	kreis.c
	gcc $(DEBUG) -Wall -c kreis.c -std=c11

flaeche.o:	flaeche.c
	gcc $(DEBUG) -Wall -c flaeche.c -lm -std=c11
	
clean:
	rm -f *.o
	rm -f *~
	
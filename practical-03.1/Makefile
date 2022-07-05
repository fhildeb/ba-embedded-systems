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
kreis:	kreis.o
	gcc $(DEBUG) -Wall -o kreis kreis.o
	
kreis.o:	kreis.c
	gcc $(DEBUG) -Wall -c kreis.c

clean:
	rm -f *.o
	rm -f *~
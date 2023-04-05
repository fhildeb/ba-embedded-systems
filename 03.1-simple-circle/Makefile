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
circle-calculation:	circle-calculation.o
	gcc $(DEBUG) -Wall -o circle-calculation circle-calculation.o
	
circle-calculation.o:	circle-calculation.c
	gcc $(DEBUG) -Wall -c circle-calculation.c

clean:
	rm -f *.o
	rm -f *~
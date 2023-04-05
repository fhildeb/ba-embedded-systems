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
circle-calculation:	area.o	circle-calculation.o
	gcc $(DEBUG) -Wall -o circle-calculation circle-calculation.o area.o -lm -std=c11

circle-calculation.o:	circle-calculation.c
	gcc $(DEBUG) -Wall -c circle-calculation.c -std=c11

area.o:	area.c
	gcc $(DEBUG) -Wall -c area.c -lm -std=c11
	
clean:
	rm -f *.o
	rm -f *~
	
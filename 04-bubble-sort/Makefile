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
bubble-sort:	bubble-sort.o	array-functions.o
	gcc $(DEBUG) -Wall -o bubble-sort bubble-sort.o array-functions.o -lm -std=c11

bubble-sort.o:	bubble-sort.c
	gcc $(DEBUG) -Wall -c bubble-sort.c -std=c11

array-functions.o:	array-functions.c
	gcc $(DEBUG) -Wall -c array-functions.c -lm -std=c11
	
clean:
	rm -f *.o
	rm -f *~
	
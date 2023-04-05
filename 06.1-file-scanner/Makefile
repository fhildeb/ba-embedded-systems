#Option zum erstellen mit
#Debug-Informationen für GDB
#Zeile 4 auskommentieren:
#DEBUG = -g

#Option zum Löschen von Objekt-
#und AutoSave-Dateien: 
#: make clean
.PHONY: clean

#Binary erstellen:
#: make
show-stat:	show-stat.o
	gcc $(DEBUG) -o show-stat show-stat.o -std=c11
	
show-stat.o:	show-stat.c
	gcc $(DEBUG) -c show-stat.c -std=c11

clean:
	rm -f *.o
	rm -f *~

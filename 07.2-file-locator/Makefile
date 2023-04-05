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
get-pathname:	get-pathname.o
	gcc $(DEBUG) -Wall -o get-pathname get-pathname.o
	
get-pathname.o:	get-pathname.c
	gcc $(DEBUG) -Wall -c get-pathname.c

clean:
	rm -f *.o
	rm -f *~

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
clone:	clone.o
	gcc $(DEBUG) -Wall -o clone clone.o
	
clone.o:	clone.c
	gcc $(DEBUG) -Wall -c clone.c

clean:
	rm -f *.o
	rm -f *~

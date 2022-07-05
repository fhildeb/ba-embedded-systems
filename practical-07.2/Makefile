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
getFullPathName:	getFullPathName.o
	gcc $(DEBUG) -Wall -o getFullPathName getFullPathName.o
	
getFullPathName.o:	getFullPathName.c
	gcc $(DEBUG) -Wall -c getFullPathName.c

clean:
	rm -f *.o
	rm -f *~

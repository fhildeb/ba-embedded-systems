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
withoutParent: withoutParent.o
	gcc $(DEBUG) -Wall -o withoutParent withoutParent.o
	
withoutParent.o:	withoutParent.c
	gcc $(DEBUG) -Wall -c withoutParent.c

clean:
	rm -f *.o
	rm -f *~
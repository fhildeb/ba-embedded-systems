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
dialog: dialog.o
	gcc $(DEBUG) -Wall -o dialog dialog.o
	
dialog.o:	dialog.c
	gcc $(DEBUG) -Wall -c dialog.c

clean:
	rm -f *.o
	rm -f *~
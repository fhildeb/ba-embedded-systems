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
starter: starter.o
	gcc $(DEBUG) -Wall -o starter starter.o
	
starter.o:	starter.c
	gcc $(DEBUG) -Wall -c starter.c

clean:
	rm -f *.o
	rm -f *~
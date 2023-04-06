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
fork-termination: fork-termination.o
	gcc $(DEBUG) -Wall -o fork-termination fork-termination.o
	
fork-termination.o:	fork-termination.c
	gcc $(DEBUG) -Wall -c fork-termination.c

clean:
	rm -f *.o
	rm -f *~
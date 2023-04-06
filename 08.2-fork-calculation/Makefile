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
fork-calculation: fork-calculation.o
	gcc $(DEBUG) -Wall -o fork-calculation fork-calculation.o
	
fork-calculation.o:	fork-calculation.c
	gcc $(DEBUG) -Wall -c fork-calculation.c

clean:
	rm -f *.o
	rm -f *~
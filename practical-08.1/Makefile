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
zaelen: zaelen.o
	gcc $(DEBUG) -Wall -o zaelen zaelen.o
	
zaelen.o:	zaelen.c
	gcc $(DEBUG) -Wall -c zaelen.c

clean:
	rm -f *.o
	rm -f *~
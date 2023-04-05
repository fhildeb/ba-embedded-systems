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
copy:	copy.o
	gcc $(DEBUG) -Wall -o copy copy.o
	
copy.o:	copy.c
	gcc $(DEBUG) -Wall -c copy.c

clean:
	rm -f *.o
	rm -f *~

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
showStat:	showStat.o
	gcc $(DEBUG) -o showStat showStat.o -std=c11
	
showStat.o:	showStat.c
	gcc $(DEBUG) -c showStat.c -std=c11

clean:
	rm -f *.o
	rm -f *~

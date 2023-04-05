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
softlink-tester: softlink-tester.o IsSoftLink.o
	gcc $(DEBUG) -o softlink-tester softlink-tester.o softlink.o -std=c11

softlink-tester.o:	softlink-tester.c softlink.h
	gcc $(DEBUG) -c softlink-tester.c -std=c11

softlink.o:	softlink.c
	gcc $(DEBUG) -c softlink.c -std=c11

clean:
	rm *.o
	rm *~
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

directory-tester: directory-tester.o lib
	gcc $(DEBUG) -Wall -o directory-tester directory-tester.o static-check.a
	
directory-tester.o:	directory-tester.c directory-check.h
	gcc $(DEBUG) -c directory-tester.c
	
lib: directory-check.o
	ar r static-check.a directory-check.o

directory-check.o:	directory-check.c
	gcc $(DEBUG) -c directory-check.c
	
clean:
	rm -f *.o
	rm -f *~

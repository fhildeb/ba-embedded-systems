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

IsDirTester: IsDirTester.o lib
	gcc $(DEBUG) -Wall -o IsDirTester IsDirTester.o libmylib.a
	
IsDirTester.o:	IsDirTester.c IsDir.h
	gcc $(DEBUG) -c IsDirTester.c
	
lib: IsDir.o
	ar r libmylib.a IsDir.o

IsDir.o:	IsDir.c
	gcc $(DEBUG) -c IsDir.c
	
clean:
	rm -f *.o
	rm -f *~

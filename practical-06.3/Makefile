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
IsSoftLinkTester: IsSoftLinkTester.o IsSoftLink.o
	gcc $(DEBUG) -o IsSoftLinkTester IsSoftLinkTester.o IsSoftLink.o -std=c11

IsSoftLinkTester.o:	IsSoftLinkTester.c IsSoftLink.h
	gcc $(DEBUG) -c IsSoftLinkTester.c -std=c11

IsSoftLink.o:	IsSoftLink.c
	gcc $(DEBUG) -c IsSoftLink.c -std=c11

clean:
	rm *.o
	rm *~
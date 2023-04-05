#Option zum erstellen mit
#Debug-Informationen für GDB
#Zeile 4 auskommentieren:
//DEBUG = -g

#Option zum Löschen von Objekt-
#und AutoSave-Dateien: 
#: make clean
.PHONY: clean

#Binary erstellen:
#: make
prak1: basic-string-output string-output-header character-count circle-calculation

basic-string-output:	basic-string-output.o
	gcc $(DEBUG) -Wall -o basic-string-output basic-string-output.o -std=c11

basic-string-output.o:	basic-string-output.c
	gcc $(DEBUG) -Wall -c basic-string-output.c -std=c11
	
string-output-header:	string-output-header.o
	gcc $(DEBUG) -Wall -o string-output-header string-output-header.o -std=c11

string-output-header.o:	string-output-header.c
	gcc $(DEBUG) -Wall -c string-output-header.c -std=c11
	
character-count:	character-count.o
	gcc $(DEBUG) -Wall -o character-count character-count.o -std=c11
	
character-count.o:	character-count.c
	gcc $(DEBUG) -Wall -c character-count.c -std=c11
	
circle-calculation:	circle-calculation.o
	gcc $(DEBUG) -Wall -o circle-calculation circle-calculation.o -std=c11	
	
circle-calculation.o:	circle-calculation.c
	gcc $(DEBUG) -Wall -c circle-calculation.c -std=c11
	
clean:
	rm -f *.o
	rm -f *~
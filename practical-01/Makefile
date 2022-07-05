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
prak1: hello1 hello2 hello3 kreis

hello1:	hello1.o
	gcc $(DEBUG) -Wall -o hello1 hello1.o -std=c11

hello1.o:	hello1.c
	gcc $(DEBUG) -Wall -c hello1.c -std=c11
	
hello2:	hello2.o
	gcc $(DEBUG) -Wall -o hello2 hello2.o -std=c11

hello2.o:	hello2.c
	gcc $(DEBUG) -Wall -c hello2.c -std=c11
	
hello3:	hello3.o
	gcc $(DEBUG) -Wall -o hello3 hello3.o -std=c11
	
hello3.o:	hello3.c
	gcc $(DEBUG) -Wall -c hello3.c -std=c11
	
kreis:	kreis.o
	gcc $(DEBUG) -Wall -o kreis kreis.o -std=c11	
	
kreis.o:	kreis.c
	gcc $(DEBUG) -Wall -c kreis.c -std=c11
	
clean:
	rm -f *.o
	rm -f *~
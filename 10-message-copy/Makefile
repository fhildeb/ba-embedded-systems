#Option zum erstellen mit
#Debug-Informationen für GDB
#Zeile 4 auskommentieren:
#DEBUG = -g

#Option zum Löschen von Objekt-
#und AutoSave-Dateien: 
#: make clean
.PHONY: clean

#Binary erstellen:ver client
server: server.c common.h
	gcc -Wall -g -o server server.c

client: client.c common.h
	gcc -Wall -g -o client client.c
	
clean:
	rm -f *.o
	rm -f *~
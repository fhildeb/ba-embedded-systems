all: client server
client: inetclient.o
	gcc -g -o client inetclient.o
server: inetserver.o
	gcc -g -o server inetserver.o
inetclient.o: inetclient.c
	gcc -Wall -g -c inetclient.c
inetserver.o: inetserver.c
	gcc -Wall -g -c inetserver.c

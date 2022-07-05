all: client server
client: inetclientUDP.o
	gcc -g -o client inetclientUDP.o
server: inetserverUDP.o
	gcc -g -o server inetserverUDP.o
inetclientUDP.o: inetclientUDP.c
	gcc -Wall -g -c inetclientUDP.c
inetserverUDP.o: inetserverUDP.c
	gcc -Wall -g -c inetserverUDP.c

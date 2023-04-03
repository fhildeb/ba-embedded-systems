all: shmserver shmclient

shmserver: shmserver.o
	gcc -g -o shmserver shmserver.o 

shmclient: shmclient.o
	gcc -g -o shmclient shmclient.o

shmserver.o: shmserver.c 
	gcc -g -c shmserver.c

shmclient.o: shmclient.c 
	gcc -g -c shmclient.c



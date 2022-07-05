all: shmserver shmclient

shmserver: shmserver.o
	gcc -g -o shmserver shmserver.o -lrt

shmclient: shmclient.o
	gcc -g -o shmclient shmclient.o -lrt

shmserver.o: shmserver.c shmbsp.h
	gcc -Wall -g -c shmserver.c

shmclient.o: shmclient.c shmbsp.h
	gcc -Wall -g -c shmclient.c

clean:
	rm -f shmserver shmclient *.o *~ core


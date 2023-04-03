all: shm-server shm-client

shmserver: shm-server.o
	gcc -g -o shm-server shm-server.o -lrt

shmclient: shm-client.o
	gcc -g -o shm-client shm-client.o -lrt

shmserver.o: shm-server.c shm.h
	gcc -Wall -g -c shm-server.c

shmclient.o: shm-client.c shm.h
	gcc -Wall -g -c shm-client.c

clean:
	rm -f shm-server shm-client *.o *~ core


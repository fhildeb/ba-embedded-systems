all: shm-sem-server shm-sem-client

shm-sem-server: shm-sem-server.o
	gcc -g -o shm-sem-server shm-sem-server.o

shm-sem-client: shm-sem-client.o
	gcc -g -o shm-sem-client shm-sem-client.o

shm-sem-server.o: shm-sem-server.c
	gcc -g -c shm-sem-server.c

shm-sem-client.o: shm-sem-client.c
	gcc -g -c shm-sem-client.c



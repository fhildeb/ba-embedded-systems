all: server client

server: server.o
	gcc -g -o server server.o -lrt -lm

client: client.o
	gcc -g -o client client.o -lrt

server.o: server.c msg.h
	gcc -Wall -g -c server.c

client.o: client.c msg.h
	gcc -Wall -g -c client.c

clean:
	rm -f server client *.o *~ core


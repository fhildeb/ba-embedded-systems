all: server client

server: server.o
	gcc -g -o server server.o -lm

client: client.o
	gcc -g -o client client.o

server.o: server.c msg.h
	gcc -g -c server.c

client.o: client.c msg.h
	gcc -g -c client.c

clean:
	rm -f server client *.o *~ core
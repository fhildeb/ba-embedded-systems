all: tcp-client tcp-server
tcp-client: tcp-client.o
	gcc -g -o tpc-client tcp-client.o
tcp-server: tcp-server.o
	gcc -g -o tcp-server tcp-server.o
tcp-client.o: tcp-client.c
	gcc -Wall -g -c tcp-client.c
tcp-server.o: tcp-server.c
	gcc -Wall -g -c tcp-server.c
clean:
	rm -f *.o tcp-client tcp-server *~
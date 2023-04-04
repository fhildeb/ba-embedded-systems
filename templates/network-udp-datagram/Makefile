all: udp-client udp-server
udp-client: udp-client.o
	gcc -g -o udp-client udp-client.o
server: udp-server.o
	gcc -g -o udp-server udp-server.o
udp-client.o: udp-client.c
	gcc -Wall -g -c udp-client.c
udp-server.o: udp-server.c
	gcc -Wall -g -c udp-server.c
clean:
	rm -f *.o udp-client udp-server *~
all: server client

server: common.h server.c
	gcc -Wall -g server.c -o $@

client: common.h client.c
	gcc -Wall -g client.c -o $@

clean: 
	rm -f server client *~ *.o

all: server client

server: common.h server.c
	gcc -Wall -g server.c -o server

client: common.h client.c
	gcc -Wall -g client.c -o client

clean: 
	rm -f server client *~ *.o

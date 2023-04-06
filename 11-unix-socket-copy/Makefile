.PHONY: clean
all: server client

server: server.c common.h
	gcc -Wall -g -o server server.c

client: client.c common.h
	gcc -Wall -g -o client client.c
	
clean:
	rm -f *.o
	rm -f *~
	rm client
	rm server
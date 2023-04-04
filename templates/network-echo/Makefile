CC = gcc
CFLAGS = -Wall -Wextra -pedantic

.PHONY: all clean

all: network-client network-server

network-client: network-client.c
	gcc -Wall -Wextra -pedantic network-client.c -o network-client

network-server: network-server.c
	gcc -Wall -Wextra -pedantic network-server -o network-server

clean:
	rm -f network-client network-server *.o *~

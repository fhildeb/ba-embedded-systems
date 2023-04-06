all: server client

server: server.c common.h
	gcc -Wall -g -o server server.c -pthread

client: client.c common.h
	gcc -Wall -g -o client client.c

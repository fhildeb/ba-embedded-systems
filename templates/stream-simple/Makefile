all: server_it server_par client

server_it: common.h server_it.c
	gcc -Wall -g server_it.c -o $@

server_par: common.h server_par.c
	gcc -Wall -g server_par.c -o $@

client: common.h client.c
	gcc -Wall -g client.c -o $@

clean: 
	rm -f server_it server_par client *~ *.o

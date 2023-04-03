all: iterative-server paralell-server client

iterative-server: common.h iterative-server.c
	gcc -Wall -g iterative-server.c -o $@

paralell-server: common.h paralell-server.c
	gcc -Wall -g paralell-server.c -o $@

client: common.h client.c
	gcc -Wall -g client.c -o $@

clean: 
	rm -f iterative-server paralell-server client *~ *.o

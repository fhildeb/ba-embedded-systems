all: shmsemserv shmsemclient

shmsemserv: shmsemserv.o
	gcc -g -o shmsemserv shmsemserv.o

shmsemclient: shmsemclient.o
	gcc -g -o shmsemclient shmsemclient.o

shmsemserv.o: shmsemserv.c
	gcc -g -c shmsemserv.c

shmsemclient.o: shmsemclient.c
	gcc -g -c shmsemclient.c



all: rwreader rwwriter rwreadersimple rwwritersimple

readerwriter.o: readerwriter.c readerwriter.h
	gcc -Wall -g -c readerwriter.c

simplesema.o: simplesema.c simplesema.h
	gcc -Wall -g -c simplesema.c

rwreader: rwreader.o readerwriter.o
	gcc -g -o rwreader rwreader.o readerwriter.o

rwwriter: rwwriter.o readerwriter.o
	gcc -g -o rwwriter rwwriter.o readerwriter.o

rwreadersimple: rwreadersimple.o simplesema.o
	gcc -g -o rwreadersimple rwreadersimple.o simplesema.o

rwwritersimple: rwwritersimple.o simplesema.o
	gcc -g -o rwwritersimple rwwritersimple.o simplesema.o

rwreader.o: rwreader.c readerwriter.h
	gcc -Wall -g -c rwreader.c

rwwriter.o: rwwriter.c readerwriter.h
	gcc -Wall -g -c rwwriter.c

rwreadersimple.o: rwreadersimple.c simplesema.h
	gcc -Wall -g -c rwreadersimple.c

rwwritersimple.o: rwwritersimple.c simplesema.h
	gcc -Wall -g -c rwwritersimple.c

clean:
	rm -f *~ *.o core rwreader rwwriter rwreadersimple rwwritersimple


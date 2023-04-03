all: advanced-reader advanced-writer simple-reader simple-writer

advanced-lock.o: advanced-lock.c advanced-lock.h
	gcc -Wall -g -c advanced-lock.c

simple-semaphore.o: simple-semaphore.c simple-semaphore.h
	gcc -Wall -g -c simple-semaphore.c

advanced-reader: advanced-reader.o advanced-lock.o
	gcc -g -o advanced-reader advanced-reader.o advanced-lock.o

advanced-writer: advanced-writer.o advanced-lock.o
	gcc -g -o advanced-writer advanced-writer.o advanced-lock.o

simple-reader: simple-reader.o simple-semaphore.o
	gcc -g -o simple-reader simple-reader.o simple-semaphore.o

simple-writer: simple-writer.o simple-semaphore.o
	gcc -g -o simple-writer simple-writer.o simple-semaphore.o

advanced-reader.o: advanced-reader.c advanced-lock.h
	gcc -Wall -g -c advanced-reader.c

advanced-writer.o: advanced-writer.c advanced-lock.h
	gcc -Wall -g -c advanced-writer.c

simple-reader.o: simple-reader.c simple-semaphore.h
	gcc -Wall -g -c simple-reader.c

simple-writer.o: simple-writer.c simple-semaphore.h
	gcc -Wall -g -c simple-writer.c

clean:
	rm -f *~ *.o core advanced-reader advanced-writer simple-reader simple-writer


#include <sys/types.h>
#include <sys/wait.h>
/* Lesen und Schreiben aus einer Pipe durch einen Prozess */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int pipefd[2], nread;
	char s[100];
	
	if(pipe(pipefd) < 0) {
	     perror("pipe");
	     return EXIT_FAILURE;
	}

	if(write(pipefd[1], "hello", 6) < 0) {
	     perror("write to pipe");
	     return EXIT_FAILURE;
	}

	nread = read(pipefd[0], s, sizeof(s));
	switch (nread) {
	   case -1:	perror("read from pipe");
	   		return EXIT_FAILURE;

	   case  0:     fprintf(stderr, "reading EOF or empty pipe\n");
	   		return EXIT_FAILURE;;

	   default:	printf("read %d bytes from pipe: %s \n", nread, s);
	}
	return EXIT_SUCCESS;
}

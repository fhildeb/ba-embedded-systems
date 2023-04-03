#include <sys/types.h>
#include <sys/wait.h>
/* 	Lesen und Schreiben aus einer Pipe,
	echte Kommunikation zwischen zwei Prozessen
	Daten werden für Kindprozess (Programm pread2) in Pipe geschrieben
	Problem: der Kindprozess erbt die Deskriptoren der Pipe,
	ausgeführtes Programm pread3 kennt sie aber nicht.
	Lösung 2: Kindprozess leitet Lesepipe vor exec() nach STDIN_FILENO um. 
	pipe-sub-input liest aus Standardeingabe
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
	int pipefd[2], neu;
	char fdstr[10];

	if(pipe(pipefd) < 0) {
	     perror("pipe");
	     return EXIT_FAILURE;
	}

	switch(fork()) {
	   case -1:	perror("fork");
	   		return EXIT_FAILURE;

	   case  0:	/* Kindprozess */

			/* Schreibpipe schließen */
			if(close(pipefd[1]) < 0) {
	   		   	perror("child: closing write pipe");
	   		   	return EXIT_FAILURE;
	   		}

			/* Standardeingabe schließen */
	   		if(close(0) < 0) {
	   		        perror("child: close stdin");
	   		        return EXIT_FAILURE;
	   		}

			/* Pipe auf stdin umleiten */
	   		if(dup(pipefd[0]) < 0) {
	   		        perror("child: dup");
	   		        return EXIT_FAILURE;
	   		}

			/* restl. Lesepipe schließen */
	   		if(close(pipefd[0]) < 0)	{
	   		        perror("child: closing old read pipe");
	   		        return EXIT_FAILURE;
	   		}

			execl("./pipe-sub-input", "pipe-sub-input", NULL);

			/* Fehler execl()) */
			perror("execl");
			return EXIT_FAILURE;

	   default:	/* Elternprozess */

			/* Lesepipe schließen */
	   		if(close(pipefd[0]) == -1) {
	                        perror("parent: closing read pipe");
	                        return EXIT_FAILURE;
	                }

			/* in Pipe schreiben */
			if(write(pipefd[1], "hello", 6) == -1) {
			        perror("parent: write");
			        return EXIT_FAILURE;
			}

			wait(NULL);
	}
	return 0;
}


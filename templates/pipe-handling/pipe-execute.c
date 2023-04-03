#include <sys/types.h>
#include <sys/wait.h>
/* 	Lesen und Schreiben aus einer Pipe,
	echte Kommunikation zwischen zwei Prozessen 
	Daten werden für Kindprozess (Programm pread2) in Pipe geschrieben
	Problem: der Kindprozess erbt die Deskriptoren der Pipe,
	ausgeführtes Programm pread2 kennt sie aber nicht.
	Lösung 1: Deskriptornummer wird als Parameter übergeben
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
	int pipefd[2];
	char fdstring[10];

	/* Pipe anlegen */
	if(pipe(pipefd) < 0) {
	     perror("pipe");
	     return EXIT_FAILURE;
	}

	/* Kindprozess erzeugen */
	switch(fork()) {
	   case -1:	perror("fork");
	   		return EXIT_FAILURE;

	   case  0:	/* Kindprozess */

			if(access("DEBUG", F_OK)==0) {
				printf("Debug mode, attach to PID %d\n", getpid());
				sleep(30);
				printf("READY!\n");
			}

			/* Schreibpipe schließen */
			if(close(pipefd[1]) < 0) {
	   		   	perror("close");
	   		   	return EXIT_FAILURE;
	   		}

			/* Deskriptornummer der Lesepipe */
			sprintf(fdstring, "%d", pipefd[0]);

			/* als Parameter an pipe-sub-argument übergeben */
			execl("./pipe-sub-argument", "pipe-sub-argument", fdstring, NULL);

			/* execl kehrt nur bei Fehler zurück */
			perror("execl");
			return EXIT_FAILURE;

	   default:	/* Elternprozess */

			/* Lesepipe schließen */
			if(close(pipefd[0]) < 0) {
	            		perror("close2");
	                	return EXIT_FAILURE;
	                }

			/* in Pipe schreiben */
			if(write(pipefd[1], "hello", 6) == -1) {
			       perror("write");
			       return EXIT_FAILURE;
			}

			/* Warte auf Ende des Kindprozesses */
			wait(NULL);
	}
	return EXIT_SUCCESS;
}


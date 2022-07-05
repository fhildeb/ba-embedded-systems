#include <sys/types.h>
#include <sys/wait.h>
/* Beispiel 1: 	Lesen und Schreiben aus einer Pipe durch einen Proze� */
/* Beachte:							      */
/* Um einen Kindprozess mit dem Debugger abzuarbeiten, ist ein Trick  */
/* notwendig (s.u.)						      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#

int main(void)
{
	int pipefd[2],
	    nread,
	    pid;

	char s[100];

	/* Erzeugen der Pipe */
	if(pipe(pipefd) < 0) {
	     perror("pipe");
	     return EXIT_FAILURE;
	}

	pid = fork();

	switch(pid) {

	    case -1:	perror("fork ");
			return EXIT_FAILURE;

	    case 0:	/* Kindprozess */

		/* wenn Datei DEBUG im akt. Verzeichnis, warte, damit
		   Debugger attached werden kann!                     */
		if(access("DEBUG", F_OK) == 0) {
			printf("In DEBUG Mode, sleeping 60s, "
			       "please attach to %d\n", getpid());
			sleep(60);
		}

		/* Schreibseite: Sclie�en */
    		if(close(pipefd[1]) < 0) {
		    perror("closing write pipe");
		    return EXIT_FAILURE;
		}

		/* Leseseite; Zeichen auslesen (max. 100 Zeichen) */
		nread = read(pipefd[0], s, sizeof(s));
		switch (nread) {
		    case -1:	perror("read");
	   			return EXIT_FAILURE;

		    case  0:	fprintf(stderr, "reading EOF or empty pipe\n");
	   			return EXIT_FAILURE;

		    default:	printf("read %d bytes from pipe: %s \n", nread, s);
		}
		break;

	    default:	/* Elternprozess */

		/* Leseseite: Sclie�en */
    		if(close(pipefd[0]) < 0) {
		    perror("closing read pipe");
		    return EXIT_FAILURE;
		}

		/* Schreibseite: String mit 6 Zeichen schreiben */
		if(write(pipefd[1], "hello", 6) < 0) {
	    	    perror("write");
	    	    return EXIT_FAILURE;
		}
		wait(NULL);
	}

	return EXIT_SUCCESS;

}




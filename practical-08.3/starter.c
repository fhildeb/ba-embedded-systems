#include <stdlib.h>
#include <stdio.h> /* printf, perror */

#include <unistd.h> /* fork, getpid, getppid */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* waitpid */

#include <string.h>

/* Praktikum 8 Aufgabe 3:
	Programm welches andere Programme welche als Argumente
	übergeben werden als Kindprozesse ausführt.
*/
int main(int argc, char** argv)
{
    if( argc < 2 )
    {
        printf("Programm muss in folgendem Schema aufgerufen werden:\n");
        printf("./starter <Programm> <Argumente>\n");
        return EXIT_FAILURE;
    }

    pid_t childProcessId, waitProcessId;

    /* Argumentvektor für Kind anlegen
    und Programme ebenso in das argv-Array kopieren.
    Wenn argv[0] dann Abbruch*/
    char** child_argv = malloc( (argc) * sizeof(char*) );

    for(int i = 1; i < argc; i++)
    {
        /* Parameter hineinkopieren */
        int laenge = strlen(argv[i])+1;
        child_argv[i-1] = malloc( laenge * sizeof(char) );
        strncpy( child_argv[i-1], argv[i], laenge );
    }

    /* Wichtig: Letztes Element der Liste MUSS null sein! */
    child_argv[argc-1] = NULL;

    childProcessId = fork();

    if(childProcessId > 0) /*Ist Elternprozess*/
    {
        int status;
        /* wait blockiert Process bis Kind fertig ist*/
        waitProcessId = wait(&status);
        printf("Kind %d hat mit %d beendet.\n", waitProcessId, status);
    }
    else if(childProcessId == 0) /*Ist Kindprozess*/
    {
        if( execvp(argv[1], child_argv) < 0 )
        {
            perror("Execvp");
            return EXIT_FAILURE;
        }
    }
    else
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
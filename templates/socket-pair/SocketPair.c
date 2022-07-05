#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>

#define LINELEN 256


int main(int argc, char **argv)
{
    int sockets[2];        /* Socketpair */

    int npid, fdin, fdout, i;
    
    char line[LINELEN];
    char **args;


    if(argc < 2) {
        fprintf(stderr, "usage: %s prog args ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        perror("Socketpair: ");
        exit(EXIT_FAILURE);
    }

    npid = fork();

    switch (npid) {
    case -1:    perror("Fork: ");
            return EXIT_FAILURE;

    case 0:        /* Kindprozess */
            
            /* Socket Elternseite schließen */            
            if (close(sockets[0]) <0) {
                perror("child: close parent socket");
                return EXIT_FAILURE;
            }
            /* Argumente für Filter bereitstellen */
            args = malloc(argc*sizeof(char*));

            for(i=1; i<argc; i++) {
                args[i-1] = argv[i];
            }
            args[argc-1]=NULL;
            
            /* Standardeingabe schließen */
            if(close(STDIN_FILENO) < 0) {
                perror("child: close stdin");
                return EXIT_FAILURE;
            }
            /* Socket von stdin umleiten */ 
            fdin = dup(sockets[1]);
            if(fdin != STDIN_FILENO) {
                perror("child: dup stdin:");
                return EXIT_FAILURE;
            }
            
            /* Standardausgabe schließen */
            if (close(STDOUT_FILENO) <0) {
                perror("child: close stdout");
                    return EXIT_FAILURE;
            }
                                                                                                    
            /* Socket nach stdout umleiten */
            fdout = dup(sockets[1]);
            if(fdout != STDOUT_FILENO) {
                perror("child: dup stdout:");
                return EXIT_FAILURE;
            }
            
            /* Socket schließen */
            if (close(sockets[1]) < 0) {
                perror("child: close");
                return EXIT_FAILURE;
            }
            /* Filter starten, liest aus stdin, schribt nach stdout */    
            execvp(argv[1], args);

    default:
            /* Elternprozess */
            
            /* Socket Kindseite schließen */            
            if (close(sockets[1]) <0) {
                perror("close child socket");
                return EXIT_FAILURE;
            }
            
            /* aus stdin lesen und zu Filter senden, Ende bei EOF */    
            fgets(line, LINELEN, stdin);
            while(!feof(stdin)) {
                write(sockets[0], line, strlen(line));
                fgets(line, LINELEN, stdin);
            }    
            
            /* Schreibseite des Socket schließen */
            if(shutdown(sockets[0], SHUT_WR) <0) {
                perror("shutdown write");
                return EXIT_FAILURE;
            }

            /* aus Socket lesen */            
            while (read(sockets[0], line, LINELEN) > 0) puts(line);
            
            if(shutdown(sockets[0], SHUT_RD) <0) {
                perror("shutdown read");
                    return EXIT_FAILURE;
            }
            
            if(close(sockets[0]) <0) {
                perror("close");
                    return EXIT_FAILURE;
            }
    }
    return EXIT_SUCCESS;
}

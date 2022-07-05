/*
 * Client baut Verbindung auf, sendet Textzeilen
 * Server setzt nach Großbuchstaben um
 * Client zeigt umgesetzte Zeile an.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include "bsp.h"


extern int errno;

int main(int argc, char* argv[])
{
    char line[100], *presult;
    FILE *sfp;
    int s, result;
    struct hostent *hp;
    struct sockaddr_in sin;

    /* Argumentzahl abtesten */
    if(argc != 2) {
        fprintf(stderr, "Nutzung: inetclient <server>\n");
        return EXIT_FAILURE;
    }

    /* Adresse des Servers bestimmen */
    if ((hp = gethostbyname(argv[1])) == NULL) {
        fprintf(stderr, "%s: unknown host.\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* Streamsocket anfordern */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("client: socket");
        return EXIT_FAILURE;
    }

    /* Serveradresse vorbereiten */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    bcopy(hp->h_addr, &sin.sin_addr.s_addr, hp->h_length);

    /* Verbindung zum Server aufbauen
     * Server muß bereits listen() aufgerufen haben! */
    if (connect(s, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("client: connect");
        return EXIT_FAILURE;
    }

    /* lesen und schreiben mit gepufferter E/A */
    sfp = fdopen(s, "r+");

    /* Der Server schickt uns zwei Zeilen mit Begrüßung und
     *  Anleitung  */
    presult=fgets(line, sizeof(line), sfp);	/* Zeile aus Socket lesen */
    assert(presult != NULL);
    puts(line);					            /* anzeigen */

    presult=fgets(line, sizeof(line), sfp);	/* Zeile aus Socket lesen */
    assert(presult != NULL);
    puts(line);					            /* anzeigen */
    if(strcmp(line, "Zu viele Clients, schließe!\n") == 0) {
        fclose(sfp);	/* Socket schließen */
        return EXIT_FAILURE;
    }
    
    presult=fgets(line, sizeof(line), stdin);	/* Zeile aus stdin lesen */
    assert(presult != NULL);

    while(strcmp(line,"ENDE\n") != 0) {		    /* solange nicht ENDE\n  */
        result=fputs(line, sfp);                 /* zum Server senden     */
        assert(result>=0);

        presult=fgets(line, sizeof(line), sfp);	/* Antwort vom Server lesen */
        assert(presult != NULL);
        puts(line);				                /* und anzeigen */

        presult=fgets(line, sizeof(line), stdin);  /* nächste Zeile aus stdin lesen */
        assert(presult != NULL);
    }

    fclose(sfp);	/* Socket schließen */

    return EXIT_SUCCESS;
}

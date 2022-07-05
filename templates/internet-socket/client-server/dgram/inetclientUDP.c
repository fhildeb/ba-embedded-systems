/*
 * Client baut Verbindung auf, sendet Textzeilen
 * Server setzt nach Großbuchstaben um
 * Client zeigt umgesetzte Zeile an.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "bsp.h"

extern int errno;

int main(int argc, char* argv[])
{
    char line[100], *presult;
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

    /* Datagrammsocket anfordern */
    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("client: socket");
        return EXIT_FAILURE;
    }

    /* Serveradresse vorbereiten */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    bcopy(hp->h_addr, &sin.sin_addr.s_addr, hp->h_length);

    /* Instruktionen vom Server anfordern
     * Server muss bereits recvfrom() aufgerufen haben! */
    if (sendto(s, "instruct", 9, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("client: sendto 1");
        return EXIT_FAILURE;
    }


    /* Der Server schickt uns zwei Zeilen mit Begrüßung und
     *  Anleitung  */
    if(recvfrom(s, line, sizeof(line), 0, NULL, NULL) < 0) { /* 1.Zeile = 1.Datagramm lesen */
		perror("recvfrom 1");
		exit(EXIT_FAILURE);
	}
    puts(line);						/* anzeigen */
	
    if(recvfrom(s, line, sizeof(line), 0, NULL, NULL) < 0) { /* 2.Zeile = 2.Datagramm lesen */
		perror("recvfrom 2");
		exit(EXIT_FAILURE);
	}
    puts(line);						/* anzeigen */

    presult=fgets(line, sizeof(line), stdin);	/* Zeile aus stdin lesen */
    assert(presult != NULL);

    while(strcmp(line,"ENDE\n") != 0) {		/* solange nicht ENDE\n  */
		if(sendto(s, line, sizeof(line), 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
			perror("client: sendto 2");
			return EXIT_FAILURE;
		}


		if(recvfrom(s, line, sizeof(line), 0, NULL, NULL) < 0) {	/* Antwort vom Server lesen */
			perror("recvfrom 3");
			exit(EXIT_FAILURE);
		}
		puts(line);				/* und anzeigen */

		presult=fgets(line, sizeof(line), stdin); /* nächste Zeile aus stdin lesen */
		assert(presult != NULL);
    }

    close(s);	/* Socket schließen */
    return EXIT_SUCCESS;
}

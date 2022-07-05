/* Nummernserver - liefert jedem Anfrager genau eine eindeutige Nummer aus.
 * - realisiert als verbindungsloser Server, 
 * - antwortet auf beliebiges Telegramm mit nächster Nummer.
 *
 * Usage: `server`   startet Server mit Nummer 0 als erster 32-Bit Nummer
 */
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUF_SIZE 100

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0);

int main (int argc, char **argv) {
    int mysock;
    struct sockaddr_un sa, peer_addr;
    socklen_t len;
    char buf[BUF_SIZE];
    int numBytes;
    int number = 0;

    /* Socket anlegen */
    mysock = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (mysock == -1)
    	handle_error("socket");

    /* entferne alten Dateieintrag für Socket, da ansonst bind() scheitert */
    remove (NUMBERS);
    
    /* Serveradresse vorbereiten und mit Socket verbinden */
    memset(&sa, 0, sizeof(sa));
    sa.sun_family = PF_LOCAL;
    strncpy(sa.sun_path, NUMBERS, sizeof(sa.sun_path) - 1);

    if (bind(mysock, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    	handle_error("bind");

    while(1) {
        /* warte auf Anfrage von Client, i
	 * die Absenderadresse wird in peer_addr und len hinterlegt */
	len = sizeof(struct sockaddr_un);
	numBytes = recvfrom(mysock, buf, BUF_SIZE, 0, 
		    (struct sockaddr *)&peer_addr, &len);
	if (numBytes == -1)
	    handle_error("recvfrom");

	/* Auswertung Anfrage, Vorbereitung Antwort */
	sprintf(buf, "%d\n", number);
	number++;

	/* sende Antwort zurück an Absender */
	if (sendto(mysock, buf, strlen(buf) + 1, 0, 
		    (struct sockaddr *)&peer_addr, len) 
		!= strlen(buf) + 1)
	    handle_error("sendto");;
    }

    return EXIT_SUCCESS;
}

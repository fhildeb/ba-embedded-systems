/* Nummernserver - liefert jedem Anfrager genau eine eindeutige Nummer aus.
 * - realisiert als iterativer Stream-Server, 
 * - sendet nächste Nummer sobald Client eine Verbindung aufbaut.
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
    int mysock, new_socket;
    struct sockaddr_un sa;
    char buf[BUF_SIZE];
    int numBytes;
    int number = 0;

    /* Socket anlegen, dient nur als 'Empfangs-Schnittstelle' für accept() */
    mysock = socket(AF_LOCAL, SOCK_STREAM, 0);
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

    /* In Empfangszustand versetzen, 5 Verbindungen können gleichzeitig 
     * beantragt werden, weitree werden abgewiesen */
    if (listen(mysock, 5) == -1)
	handle_error("listen");

    while(1) {
	/* warte auf neue Verbindung --  */
	new_socket = accept(mysock, NULL, NULL);
	if (new_socket == -1)
	    handle_error("accept");

	/* Auswertung Anfrage, Vorbereitung Antwort */
	sprintf(buf, "%d\n", number);
	number++;

	/* sende Antwort zurück an Absender */
	numBytes= write(new_socket, buf, strlen(buf)+1);
	if (numBytes != strlen(buf)+1)
	    handle_error("write");

	close(new_socket);
    }

    return EXIT_SUCCESS;
}

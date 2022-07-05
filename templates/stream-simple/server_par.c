/* Nummernserver - liefert jedem Anfrager genau eine eindeutige Nummer aus.
 * - realisiert als paralleler Stream-Server, 
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
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 100

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0);


/* signal handler for SIGCHLD to read return status of child processes */
void read_chldstate(int signo) {
    /* read state of all finished children to avoid zombies */
    while (waitpid(-1, NULL, WNOHANG) > 0)
	;
}


int main (int argc, char **argv) {
    int mysock, new_socket;
    int npid;
    struct sockaddr_un sa;
    char buf[BUF_SIZE];
    int numBytes;
    int number = 0;
    struct sigaction new_action;

    /* Socket anlegen als 'Empfangs-Schnittstelle' für accept() */
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
     * beantragt werden, weitere werden abgewiesen */
    if (listen(mysock, 5) == -1)
	handle_error("listen");

    /* Installiere Signalhandler für SIGCHLD zur Vermeidung von Zombies */
    memset(&new_action, 0, sizeof(new_action));
    new_action.sa_handler = read_chldstate;

    if (sigaction(SIGCHLD, &new_action, NULL) == -1)
	handle_error("sigaction");


    while(1) {
	/* warte auf neue Verbindung --  */
	new_socket = accept(mysock, NULL, NULL);
	if (new_socket == -1) {
	    if (errno != EINTR) 
		handle_error("accept");

	    /* falls durch SIGCHLD unterbrochen -> erneut starten */
	    continue;
	}

	/* Auswertung Anfrage, Vorbereitung Antwort 
	 * !!! muss VOR fork() geschehen !!! */
	sprintf(buf, "%d\n", number);
	number++;

	npid = fork();

	switch (npid) {
	    case -1:
		handle_error("fork");
		break;

	    case 0: // Kind, kümmert sich um 'new_socket'
		close(mysock);		/* schließe Anmeldungssocket */

		/* sende Antwort zurück an Absender */
		numBytes= write(new_socket, buf, strlen(buf)+1);
		if (numBytes != strlen(buf)+1)
		    handle_error("write");

		close(new_socket); 
		exit( EXIT_SUCCESS);	/* Prozess zu Ende */

	    default: // Elternprozess
		close(new_socket);
		break;			/* warte auf neue Connection */
	}
    }

    return EXIT_SUCCESS;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <strings.h>  // für bcopy
#include <ctype.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "bsp.h"

/* Protokoll: 
   Client sendet "instruct"
   Server sendet zwei Pakete mit Instruktionen
   jede andere Zeichenkette wird in Großbuchstaben
   konvertiert und zurückgesandt */


extern int errno;
int s;	// Socket

// Bei Serverabbruch Socket schließen!
void exithandler()
{
   close(s);
   return;
}

// Bei Abbruch mit CTRL+C Socket schließen
void sighandler(int signo)
{
   exithandler();
   _exit(EXIT_SUCCESS);
}


int main(void)
{
    char line[100];

    socklen_t fromlen;
    int i,
	result;
	struct sigaction old, new;

	/* Exit-Handler installieren */
    atexit(exithandler);

    /* Signalbehandlung vorbereiten und installieren */
    sigemptyset(&(new.sa_mask));
    new.sa_handler = sighandler;
    new.sa_flags = 0;

    if(sigaction(SIGINT, &new, &old) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

	
    struct sockaddr_in sockin, peerin;

    /* Datagrammsocket holen  */
    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("server: socket");
        exit(EXIT_FAILURE);
    }

    /* Adresse für Server erzeugen */
    sockin.sin_family = AF_INET;
    sockin.sin_port = htons(PORT);
    sockin.sin_addr.s_addr = htonl(INADDR_ANY);


    /* Adresse binden */
    if (bind(s, (struct sockaddr *) &sockin, sizeof(sockin)) < 0) {
        perror("server: bind");
        exit(EXIT_FAILURE);
    }


    while(1) {

        /* Empfange ein Datagramm  */
        fromlen = sizeof(peerin);

        if ((result = recvfrom(s, line, sizeof(line), 0, (struct sockaddr *) &peerin, &fromlen)) < 0) {
			if(errno == EINTR) continue;
			perror("server: accept");
    	    exit(EXIT_FAILURE);
		}	

		/* auf Serverkonsole melden */
		result=fprintf(stderr, "Datagram from %s Port %d\n",
						inet_ntoa(peerin.sin_addr), 
						ntohs(peerin.sin_port)
					  );
		assert(result > 0);


		if(strcmp(line, "instruct") == 0) {	 	/* erste Anforderung ?? */
			result=snprintf(line,
							sizeof(line),
							"Hallo %s Port %d\n", 
							inet_ntoa(peerin.sin_addr),
							ntohs(peerin.sin_port)
						   );
			assert(result >=0);
		
			result=sendto(s, 
							line, 
							strlen(line)+1, 
							0, 
							(struct sockaddr *)&peerin, 
							fromlen
						 );
			assert(result >=0);
			strncpy(line, "gib Zeilen ein, Ende ist ENDE\n", sizeof(line));
		
			result=sendto(s, 
							line, 
							strlen(line)+1, 
							0, 
							(struct sockaddr *)&peerin, 
							fromlen
						 );
			assert(result >=0);
	
		} else {				/* alle anderen Anforderungen: Zeilen */
			for(i=0; i<strlen(line); i++) {
				line[i]=toupper(line[i]);
			}
			result=sendto(s, 
							line, 
							strlen(line)+1, 
							0, 
							(struct sockaddr *)&peerin, 
							fromlen
						);
			assert(result >=0);
		}

   }
   return EXIT_SUCCESS;
}

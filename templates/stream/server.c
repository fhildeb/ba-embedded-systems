#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include "sockbsp.h"
#include <signal.h>
#include <math.h>

/* global wegen Signalhandler */
int sockfd;

/* bei SIGINT und Serverende wird Socket gelöscht */
/* d.h. auch im Fehlerfall                        */

void exithandler()
{
   if(close(sockfd) < 0) {
     perror("close");
     _exit(EXIT_FAILURE);
   }

   if(unlink(ORDER_SOCKET) < 0) {
     perror("unlink");
     _exit(EXIT_FAILURE);
   }
   return;
}

void sighandler(int signo)
{
   exithandler();
   _exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
    struct sockaddr_un server_addr, client_addr;
    int addrlen;
    socklen_t clientaddrlen;
    int commsockfd;
    int nread, nwritten;
    struct sigaction old, new;
    struct order order;
    struct result result;  

    /* korrekten Aufruf sichern */
    if(argc != 1) {
        fprintf(stderr, "Aufruf: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    /* Socket anlegen */
    if((sockfd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Adresse vorbereiten */
    server_addr.sun_family = AF_LOCAL;  
    strncpy(server_addr.sun_path,  ORDER_SOCKET, sizeof(server_addr.sun_path)-1);
    // addrlen = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);
    addrlen = strlen(server_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);
    unlink(ORDER_SOCKET);

    /* Adresse binden */
    if(bind(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* Zugriffsrechte auf socket werden von umask abgeleitet!! */
    /* Jeder Nutzer soll diesen Socket verwenden können        */
    if(chmod(ORDER_SOCKET, 0777) < 0) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }

    /* Warteschlange initialisieren */
    if(listen(sockfd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

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

    /* Server-Schleife, ende durch ctrl+c */
    while (1) {
        /* Länge der Struktur für Peer */
        clientaddrlen = sizeof(struct sockaddr_un);

        /* auf Auftrag warten */
        if((commsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clientaddrlen)) < 0) {
              perror("accept");
              exit(EXIT_FAILURE);
        }

        /* Auftrag lesen */
        if((nread = read(commsockfd, &order, sizeof(struct order))) < 0) {
              perror("read");
              exit(EXIT_FAILURE);
        }

        /* Ergebnis vorbereiten */

        result.argument = order.argument;
        result.functno = order.functno;

        switch(order.functno) {

                case sinus:
                   result.result = sin(order.argument);
                   break;

                case cosinus:
                   result.result = cos(order.argument);
                   break;


                case tangens:
                   result.result = tan(order.argument);
                   break;

                default:
                   fprintf(stderr, "falsche Funktionsnummer\n");
                   result.result = 0.0;

        }

         /* Ergebnis senden */
        if((nwritten = write(commsockfd, &result, sizeof(struct result))) < 0) {
              perror("write");
              exit(EXIT_FAILURE);
        }

        /* Kommunikationssocket schließen */
        if(close(commsockfd) < 0) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }
}

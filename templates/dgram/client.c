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
#define SOCKNAME_LEN 20

int sockfd;
char client_sock_name[SOCKNAME_LEN];

void exithandler()
{
   if(close(sockfd) < 0) {
     perror("close");
     _exit(EXIT_FAILURE);
   }

   if(unlink(client_sock_name) < 0) {
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
    int res;
    socklen_t addrlen;
    int nwritten, nread;
    struct order order;
    struct result result;
    struct sockaddr_un server_addr, server_addr2, client_addr;
    struct sigaction old, new;
    
    if(argc != 1) {
        fprintf(stderr, "Aufruf: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    /* Socket anlegen */
    if((sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* eigene Adresse vorbereiten  */
    client_addr.sun_family = AF_LOCAL;
    snprintf(client_addr.sun_path, sizeof(client_addr.sun_path)-1,"%s%05d" , CLIENT_SOCKET, getpid());
    addrlen = strlen(client_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);
    strncpy(client_sock_name, client_addr.sun_path, SOCKNAME_LEN-1);
    
    /* eigene Adresse binden */
    if(bind(sockfd, (struct sockaddr *)&client_addr, addrlen) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* Zugriffsrechte auf socket werden von umask abgeleitet!! */
    /* Jeder Nutzer soll diesen Socket verwenden können        */
    if(chmod(client_addr.sun_path, 0777) < 0) {
        perror("chmod");
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

    /* Serveradresse vorbereiten */
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path,  ORDER_SOCKET, sizeof(server_addr.sun_path)-1);
    addrlen = strlen(server_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);


    /* Aufruf-Schleife, Abbruch durch ctrl+c */
    while (1) {

      /* Funktion abfragen */
      do {
        printf("Funktion eingeben (sin=0, cos=1, tan=2 ende=3): ");
        res = scanf("%d",&order.functno);
        while(getchar()!='\n');
      } while (res != 1);


      /* Fertig ? */
      if(order.functno == 3) {
            exit(EXIT_SUCCESS);
      }

      do {
        printf("Argument: ");
        res = scanf(" %lg", &order.argument);
        while(getchar()!='\n');
      } while (res != 1);

    
      /* Auftrag senden */
      if((nwritten = sendto(sockfd, &order, sizeof(struct order), 0, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0) {
         perror("write");
         exit(EXIT_FAILURE);
      }

      /* auf Ergebnis warten */
      if((nread = recvfrom(sockfd, &result, sizeof(struct result), 0, (struct sockaddr *)&server_addr2, &addrlen)) < 0) {
         perror("read");
         exit(EXIT_FAILURE);
      }

      printf("Ergebnis: %s(%g) = %g \n", s[order.functno], 
                                   order.argument,   result.result);

    }

    /* Socket schließen */
    if(close(sockfd) < 0) {
         perror("close");
         exit(EXIT_FAILURE);
    }
}




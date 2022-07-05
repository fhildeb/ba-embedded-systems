#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include "sockbsp.h"
#include <signal.h>
#include <math.h>


int main(int argc, char *argv[])
{
    int sockfd;
    int res, addrlen;
    int nwritten, nread;
    struct order order;
    struct result result;
    struct sockaddr_un server_addr;

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

     /* Verbindung herstellen */
     if(connect(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
      }
    
      /* Auftrag senden */
      if((nwritten = write(sockfd, &order, sizeof(struct order))) < 0) {
         perror("write");
         exit(EXIT_FAILURE);
      }

      /* auf Ergebnis warten */
      if((nread = read(sockfd, &result, sizeof(struct result))) < 0) {
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




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include "msg.h"
#include <signal.h>
#include <math.h>



/* global wegen Signalhandler */
mqd_t mqordid;

/* bei SIGINT und Serverende wird Messagequeue gelöscht */
/* d.h. auch im Fehlerfall                              */

void exithandler()
{
   if(mq_close(mqordid) < 0) {
     perror("mq_close");
     _exit(EXIT_FAILURE);
   }

   if(mq_unlink(ORDER_MQ) < 0) {
     perror("mq_unlink");
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
    mqd_t mqresid;
    struct sigaction old, new;
    struct order order;
    struct result result;  

    /* Flags, maxmsg, maxsize, no_in_queue */
    struct mq_attr order_attr = {0, 8, sizeof(order), 0};
 

    /* Messagequeue öffnen */
    if((mqordid = mq_open(ORDER_MQ, O_CREAT|O_RDONLY, 0600, &order_attr)) < 0) {
        perror("mqopen order");
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

        /* auf Auftrag warten */
        if(mq_receive(mqordid, (char *)&order, sizeof(struct order), 
                                                   NULL) < 0) {
              perror("mq_receive");
              exit(EXIT_FAILURE);
        }

        /* Ergebnismessage vorbereiten */
       
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

        
        /* Ergebnis Messagequeue (vom Client angelegt) öffnen */
        if((mqresid = mq_open(order.res_queue, O_WRONLY)) < 0) {
            perror("mq_open result");
            exit(EXIT_FAILURE);
        }        

        /* Ergebnis senden */
        
        if(mq_send(mqresid, (char *)&result, sizeof(struct result), 1) < 0) {
                   perror("mq_send");
        }

    }
}

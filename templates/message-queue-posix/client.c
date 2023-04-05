#include "msg.h"
#include <math.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

mqd_t mqresid;
char mqname[20];

void exithandler()
{
  if (mq_close(mqresid) < 0)
  {
    perror("mq_close");
    _exit(EXIT_FAILURE);
  }

  if (mq_unlink(mqname) < 0)
  {
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
  mqd_t mqordid;
  int res;

  struct sigaction old, new;
  struct order order;
  struct result result;
  /* Flags, maxmsg, maxsize, no_in_queue */
  struct mq_attr result_attr = {0, 2, sizeof(struct result), 0};

  if (argc != 1)
  {
    fprintf(stderr, "Aufruf: %s\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Auftragsmessagequeue zum Schreiben öffnen */
  if ((mqordid = mq_open(ORDER_MQ, O_WRONLY)) < 0)
  {
    perror("mq_open order");
    exit(EXIT_FAILURE);
  }

  /* Ergebnissmessagequeue vorbereiten */
  sprintf(order.res_queue, RESULT_MQ "%05d", getpid());
  strncpy(mqname, order.res_queue, sizeof(mqname) - 1);

  /* Ergebnissmessagequeue anlegen, zum Lesen öffnen */
  if ((mqresid = mq_open(order.res_queue, O_CREAT | O_RDONLY, 0600,
                         &result_attr)) < 0)
  {
    perror("mqopen result");
    exit(EXIT_FAILURE);
  }

  /* Exit-Handler installieren */
  atexit(exithandler);

  /* Signalbehandlung vorbereiten und installieren */
  sigemptyset(&(new.sa_mask));
  new.sa_handler = sighandler;
  new.sa_flags = 0;

  if (sigaction(SIGINT, &new, &old) < 0)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  /* Aufruf-Schleife, Abbruch durch ctrl+c */
  while (1)
  {

    /* Funktion abfragen */
    do
    {
      printf("Funktion eingeben (sin=0, cos=1, tan=2 ende=3): ");
      res = scanf("%d", &order.functno);
      while (getchar() != '\n')
        ;
    } while (res != 1);

    /* Fertig ? */
    if (order.functno == 3)
    {
      exit(EXIT_SUCCESS);
    }

    do
    {
      printf("Argument: ");
      res = scanf(" %lg", &order.argument);
      while (getchar() != '\n')
        ;
    } while (res != 1);

    /* aUFTRAG SENDEN */
    if (mq_send(mqordid, (char *)&order, sizeof(struct order), 1) < 0)
    {
      perror("mq_send");
      exit(EXIT_FAILURE);
    }

    /* auf Ergebnis warten */
    if (mq_receive(mqresid, (char *)&result, sizeof(struct result), NULL) < 0)
    {
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }

    printf("Ergebnis: %s(%g) = %g \n", s[order.functno], order.argument,
           result.result);
  }
}

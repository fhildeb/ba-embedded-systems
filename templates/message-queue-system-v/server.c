#include "msg.h"
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

/* global wegen Signalhandler */
int msqid;

/* bei SIGINT und Serverende wird Messagequeue gelöscht */
/* d.h. auch im Fehlerfall                              */

void exithandler()
{

  if (msgctl(msqid, IPC_RMID, NULL) < 0)
  {
    perror("msgctl");
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
  key_t key;

  struct sigaction old, new;

  struct auftrag_msg auftrag_msg;
  struct ergebnis_msg ergebnis_msg;

  /* Signalbehandlung vorbereiten */
  sigemptyset(&(new.sa_mask));
  new.sa_handler = sighandler;
  new.sa_flags = 0;

  if (sigaction(SIGINT, &new, &old) < 0)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  /* Key für Messagequeue ermitteln */
  if ((key = ftok(argv[0], 1)) < 0)
  {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  /* Messagequeue öffnen */
  if ((msqid = msgget(key, IPC_CREAT | 0600)) < 0)
  {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  /* Exit-Handler installieren (löschen der Queue) */
  atexit(exithandler);

  /* Server-Schleife, ende durch ctrl+c */
  while (1)
  {

    /* auf Auftrag warten */
    if (msgrcv(msqid, &auftrag_msg, sizeof(struct auftrag), AUFTRAGS_MSGTYP,
               0) < 0)
    {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }

    /* Ergebnismessage vorbereiten */
    ergebnis_msg.mtype = auftrag_msg.auftrag.erg_msgtyp;
    ergebnis_msg.ergebnis.argument = auftrag_msg.auftrag.argument;
    ergebnis_msg.ergebnis.funktno = auftrag_msg.auftrag.funktno;

    switch (auftrag_msg.auftrag.funktno)
    {

    case sinus:
      ergebnis_msg.ergebnis.resultat = sin(auftrag_msg.auftrag.argument);
      break;

    case cosinus:
      ergebnis_msg.ergebnis.resultat = cos(auftrag_msg.auftrag.argument);
      break;

    case tangens:
      ergebnis_msg.ergebnis.resultat = tan(auftrag_msg.auftrag.argument);
      break;

    default:
      fprintf(stderr, "falsche Funktionsnummer\n");
      ergebnis_msg.ergebnis.resultat = 0.0;
    }

    /* Ergebnis senden */
    if (msgsnd(msqid, &ergebnis_msg, sizeof(struct ergebnis), 0) < 0)
    {
      perror("msgsnd");
    }
  }
}

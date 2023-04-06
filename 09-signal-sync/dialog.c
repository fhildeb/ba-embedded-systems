#include <stdlib.h>

#include <signal.h>    /* kill() */
#include <stdio.h>     /* printf(), perror() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid() */
#include <unistd.h>    /* fork(), getpid(), pause() */

/*  Praktikum 9 Aufgabe 1:
        Programm welches im Wechsel die PID des Eltern sowie des
        erstellten Kindprozesses ausgibt.
*/
void signalhandler(int signal)
{
  /* hier kein code, signalhandler nur nötig,
   * weil pause() auf terminierung oder signalhandler
   * wartet. Nicht auf andere signale.
   */
  return;
}

int main(int argc, char **argv)
{
  pid_t processid, childprocessid;
  struct sigaction handler;
  struct sigaction oldSigAction;

  /* sigaction initialisieren */
  handler.sa_handler = signalhandler;
  handler.sa_flags = 0;

  /* leere Signalmaske (nichts außer SIGUSR1 blockieren) */
  sigemptyset(&handler.sa_mask);

  /*
   * signalhandler installieren
   * VOR fork()! Wird vererbt!
   */
  if (sigaction(SIGUSR1, &handler, &oldSigAction) < 0)
  {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  childprocessid = fork(); /* erzeuge Kindprozess*/

  if (childprocessid > 0) /* falls Elternprozess*/
  {
    sleep(1); /* siehe Praktikums-Tipp */

    processid = getpid(); /* eigene PID */

    for (int i = 0; i < 20; i++)
    {
      printf("Elternprozess PID: %d\n", processid); /* printe zuerst */
      kill(childprocessid, SIGUSR1);                /* signalisiere Kind */
      pause(); /* warte auf SIGUSR1 von Kindprozess */
    }

    int cstatus;
    waitpid(childprocessid, &cstatus, 0); // auf ende des kindes warten.
  }
  else if (childprocessid == 0) /* falls Kindprozess */
  {
    pid_t parentprocessid;

    processid = getpid();        /* eigene PID */
    parentprocessid = getppid(); /* parent PID */

    for (int i = 0; i < 20; i++)
    {
      pause(); /* warte auf SIGUSR1 von Elternprozess */
      printf("Kindprozess   PID: %d\n", processid); /* printe als zweites */
      kill(parentprocessid, SIGUSR1);               /* signalisiere Parent */
    }
  }
  else /* falls Fehler */
  {
    perror("Fork");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
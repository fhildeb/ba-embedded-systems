/* Beispiel 7: Demonstriert das Setzen einer Signalmaske und die Abfrage
 * evtl. anhängiger Signale
 */
#include <errno.h>   
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct sigaction act, oldact;

static void sig_int(int sig);

int main(void)
{
     sigset_t  newmask, oldmask, pendmask;
     int rest;
     
     /* installiere Handler für SIGINT-Signal */
     act.sa_handler=sig_int;
     /* leere Signalmaske: andere Signale werden akzeptiert */
     sigemptyset(&act.sa_mask);
     act.sa_flags=0;
     
     if (sigaction(SIGINT, &act, &oldact) < 0)
          perror("can't catch SIGINT");

     /* bereite Signalmaske mit SIGINT vor */
     sigemptyset(&newmask);
     sigaddset(&newmask, SIGINT);
     
     /* blockiere SIGINT, rette aktuelle Signalmaske */
     if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
          perror("SIG_BLOCK error");

     /* ein jetzt eintreffendes INT-Signal bleibt anhaengig */
     sleep(5);     

     /* schreibe Menge der anhaengigen Signale nach pendmask */	
     if (sigpending(&pendmask) < 0)
          perror("sigpending error");

          /* ist SIGINT anh?ngig ? */ 	  
     if (sigismember(&pendmask, SIGINT))
          printf("\nSIGINT pending\n");

     /* stelle alte Signalmaske her                          */
     /* ein anhaengiges SIGINT loest jetzt Signalhandler aus */
     if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
          perror("SIG_SETMASK error");
     printf("SIGINT unblocked\n");

     /* wenn Signalhandler noch aktiv ist, beendet das n?chste 
        SIGINT sleep(), sonst wird das Programm abgebrochen */
     rest = sleep(10);     
     printf("Programmende: %d\n", rest);
     exit(0);
}

static void sig_int(int signo)
{
     printf("caught SIGINT\n");
     /* stelle alte Signalreaktion wieder her */
     if (sigaction(SIGINT, &oldact, &act) <0 )
          perror("can't reset SIGINT");
     return;
}

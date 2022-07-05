/* Beispiel 8:                                                      */
/* Quelle: Stevens: Advanced programming in the UNIX Environment    */

#include  <signal.h>
#include  <setjmp.h>
#include  <time.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <errno.h>
#include  <unistd.h>

static void sig_usr1(int sig), sig_alrm(int sig);
void pr_mask(const char *str);

static sigjmp_buf jmpbuf;

static volatile sig_atomic_t canjump;   /* wird durch Signalhandler geändert. */

int main(void)
{
     struct sigaction sa_usr1, sa_usrold, sa_alarm, sa_alarmold;
     
     sa_usr1.sa_handler=sig_usr1;
     sigemptyset(&sa_usr1.sa_mask);
     sa_usr1.sa_flags=0;
     
     sa_alarm.sa_handler=sig_alrm;
     sigemptyset(&sa_alarm.sa_mask);
     sa_alarm.sa_flags=0;
     
     if (sigaction(SIGUSR1, &sa_usr1, &sa_usrold) < 0) {
          perror("signal(SIGUSR1) error");
          exit(1);
     }
     if (sigaction(SIGALRM, &sa_alarm, &sa_alarmold) < 0) {
          perror("signal(SIGALRM) error");
          exit(1);
     }
     
     pr_mask("starting main: ");             /* {Prog prmask} */

     if (sigsetjmp(jmpbuf, 1)) {
          pr_mask("ending main: ");
          exit(0);
     }
     canjump = 1;                       /* now sigsetjmp() is OK */

     for ( ; ; )
          pause();
}


static void sig_usr1(int signo)
{
     time_t    starttime;

     if (canjump == 0)
          return;             /* unexpected signal, ignore */

     pr_mask("starting sig_usr1: ");

     alarm(10);                /* SIGALRM in 10 seconds */

     starttime = time(NULL);
     for ( ; ; )               /* busy wait for 15 seconds */
          if (time(NULL) > starttime + 15)
               break;

     pr_mask("finishing sig_usr1: ");

     canjump = 0;
     siglongjmp(jmpbuf, 1);   /* jump back to main, don't return */
}

static void sig_alrm(int signo)
{
     pr_mask("in sig_alrm: ");
     return;
}

/* Gib nutzerdefinierte Zeichenkette str und Signalmaske aus */

void pr_mask(const char *str)
{
     sigset_t  sigset;
     int       errno_save;

     errno_save = errno;      /* we can be called by signal handlers */
     if (sigprocmask(0, NULL, &sigset) < 0)
          perror("sigprocmask error");

     printf("%s", str);
     if (sigismember(&sigset, SIGINT))  printf("SIGINT ");
     if (sigismember(&sigset, SIGQUIT)) printf("SIGQUIT ");
     if (sigismember(&sigset, SIGUSR1)) printf("SIGUSR1 ");
     if (sigismember(&sigset, SIGALRM)) printf("SIGALRM ");
          /* remaining signals can go here */
     printf("\n");
     errno = errno_save;
}

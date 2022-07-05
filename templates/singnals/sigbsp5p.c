/* Demonstriert das Erzeugen eines Kindprozesses und die Auswertung 
 * des SIGCLD-Signals bei dessen Ende
 */
#define _XOPEN_SOURCE
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>

void sig_cld(int signo);

int main(void)
{
    pid_t   pid;
    int     i;
    
    struct sigaction act, oldact;
    
    act.sa_handler=sig_cld;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
        
    if(sigaction(SIGCLD, &act, &oldact) < 0) {
            printf("signal error\n");
            exit(1);
    }
            
    for (i=0; i < 10; i++){
       if ( (pid = fork()) < 0) {
           printf("fork error\n");
           exit(1);
       }
       else if (pid == 0) {     /* child */
           sleep(2);
           exit(i);
       }
       pause();      		/* parent */
    }
	
    exit(0);
}

void sig_cld(int signo)
{
    pid_t   pid;
    int     status;
    printf("SIGCLD received, ");
    if ((pid = wait(&status)) < 0) {        /* fetch child status */
        printf("wait error\n");
        exit(1);
    }
    printf("pid = %d  Status: %d\n", pid, WEXITSTATUS(status));
    return;                                 /* interrupts pause() */
}

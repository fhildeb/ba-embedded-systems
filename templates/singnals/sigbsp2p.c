/* Beispiel 2: Installation eines SIGINT - Handlers  
   Handler fängt ersten SIGINT ab, danach Rückstellen auf Standardreaktion
*/
#include <stdio.h> 
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

struct sigaction act, oldact;

void handler (int signo);
int main(void)
{       
    /* neuer Signalhandler */
    act.sa_handler=handler;
    /* keine anderen Signale sperren */
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_ONESHOT; 

    /* Signalhandler installieren                   */
    if (sigaction(SIGINT, &act, &oldact) < 0) {
        perror("installing signalhandler");
        return 1;
    }

    for(;;) 
        pause();

    return 0;
}

/* Signalhandler */
void handler(int signo)
{
    printf("Signal %d catched\n", signo);
}


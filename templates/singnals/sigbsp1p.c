/* Beispiel 1: Ignorieren von SIGINT  */
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    struct sigaction act, oldact;
        
    /* Handler festlegen -> ignoriere Signal */
    act.sa_handler = SIG_IGN;
    /* kein anderes Signal maskieren */
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    sigaction(SIGINT, &act, &oldact);
    puts("SIGINT disabled");

    /* hier wird jedes ^C ignoriert */
    sleep(5);

    /* alten Zustand wiederherstellen */
    sigaction(SIGINT, &oldact, &act);
    puts("SIGINT enabled");

    pause();    /* pause() wartet auf Signal */

    return 0;        
}

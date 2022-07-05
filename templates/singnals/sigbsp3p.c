/* Beispiel 3: Installation eines SIGINT - Handlers 
 *  zeigt u.a., dass blockierte Signale keine Warteschlangen  haben
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

struct sigaction act, oldact;

void handler (int signo);
int main(void)
{
        act.sa_handler = handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        
	/* Signalhandler installieren                   */
	if (sigaction(SIGINT, &act, &oldact) < 0) {
                perror("installing sighandler");
                exit(1);
        }

        /* Warteschleife                                */
	for(;;) pause();
}

/* Signalhandler */
void handler(int signo)
{
// die "Totzeit" im Signalhandler wird künstlich
// vergrößert, um schnell hintereinander eintreffende Signale 
// simulieren zu können.
// Wird während dieser Zeit mehrfach ^C gedrückt, wird der Handler nach Ablauf
// der Zeit nur einmal aufgerufen.
  
	printf("Signal %d catched\n", signo);
	sleep(2);
	printf("fertig\n");
}

/* Beispiel 5: Timeout - Behandlung
   Handler springt nach Signalbehandlung an mit setjmp() registrierte
   Stelle zurück
*/
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void timeout (int signo);
/* jmp - Buffer für setjmp */
sigjmp_buf  env;

int main(void)
{
	char buf[16];
        struct sigaction act, oldact;

        act.sa_handler = timeout;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;

	sigaction(SIGALRM, &act, &oldact);

	if (sigsetjmp(env,1) == 0) {
	/* wird bei Installation des setjmp() ausgeführt: Alarm in 5 Sec */
		alarm(5);
		printf("Type a word; if You don't in 5 ");
		printf("seconds I'll use \"WORD\": ");
		fgets(buf, sizeof(buf), stdin);

		/* Alarm ausschalten */
		alarm(0);
	} else {
		/* wird nach Alarmsignal und Handler ausgeführt */
		strcpy(buf, "WORD");
	}
	printf("\nThe word is %s\n", buf);
	return (0);
}

/* Signalhandler */
void timeout(int signo)
{
	/* Hier können Anweisungen zur Timeoutbehandlung stehen */
	printf("Signal %d catched\n", signo);

	/* Springe nach setjmp, gib 1 zurück */
	siglongjmp(env, 1);
}


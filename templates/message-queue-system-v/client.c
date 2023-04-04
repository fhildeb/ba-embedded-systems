#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"
#include <signal.h>
#include <math.h>



int main(int argc, char *argv[])
{
	key_t key;
	int msqid, res;

	struct auftrag_msg auftrag_msg;
	struct ergebnis_msg ergebnis_msg;

	if(argc != 2) {
		fprintf(stderr, "Aufruf: client <servername>");
		exit(EXIT_FAILURE);
	}

	if((key = ftok(argv[1], 1)) < 0) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	if((msqid = msgget(key, 0600)) < 0) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}


	/* Aufruf-Schleife, Abbruch durch ctrl+c */
	while (1) {

                do {
                    printf("Funktion eingeben (sin=0, cos=1, tan=2 ende=3): ");
                    res=scanf(" %d",&auftrag_msg.auftrag.funktno);
                    while(getchar()!='\n');
                } while (res != 1 || auftrag_msg.auftrag.funktno < 0  || auftrag_msg.auftrag.funktno > 3);

                if(auftrag_msg.auftrag.funktno == 3)
			exit(EXIT_SUCCESS);

		do {
                    printf("Argument: ");
                    res=scanf(" %lg", &auftrag_msg.auftrag.argument);
                    while(getchar()!='\n');
                } while(res != 1);

		/* Ergebnismessagetyp muss eindeutig sein -> PID  */

		auftrag_msg.auftrag.erg_msgtyp = getpid();

                auftrag_msg.mtype = AUFTRAGS_MSGTYP;

                if(msgsnd(msqid, &auftrag_msg, sizeof(struct auftrag), 0) < 0) {
                   perror("msgsnd");
                }

		/* auf Ergebnis warten */
		if(msgrcv(msqid, &ergebnis_msg, sizeof(struct ergebnis), auftrag_msg.auftrag.erg_msgtyp, 0) < 0) {
                   	perror("msgrcv");
                        exit(EXIT_FAILURE);
		}

		printf("Ergebnis: %s(%g) = %g \n", s[auftrag_msg.auftrag.funktno], auftrag_msg.auftrag.argument,
			ergebnis_msg.ergebnis.resultat);

        }
}




#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>


/* Kopie aus Manual semctl(2) */ 
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
       /* union semun is defined by including <sys/sem.h> */
#else
       /* according to X/OPEN we have to define it ourselves */
       union semun {
               int val;                 	/* value for SETVAL */
               struct semid_ds *buf; 		/* buffer for IPC_STAT, IPC_SET */
               unsigned short int *array; 	/* array for GETALL, SETALL */
               struct seminfo *__buf;   	/* buffer for IPC_INFO */
       };
#endif
                                                                          
/* Semaphor anlegen und auf value initialisieren  */ 
int InitSema(char *name, int value)
{
	int semid;
	union semun args;
	key_t key;
	
	key = ftok(name, 1);
	semid = semget(key, 1, IPC_CREAT|IPC_EXCL|0600);
	
	if(semid >=0) {			/* wenn korrekt angelegt */
		args.val=value;		/* mit value initialisieren */
		if(semctl(semid, 0, SETVAL, args) == -1) {
			perror("semctl");
			exit(1);
		}
		return semid;	
	} 

	if (errno == EEXIST)  {	   /* wenn Semaphor bereits existiert */	
		semid = semget(key, 1, 0);	/* öffnen */
		return semid;
	} else  {
		return (-1);
	}	
}
		
void P(int semid)  /* Semaphor anfordern */
{
	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_op=-1;
	buf.sem_flg=0;
	if(semop(semid, &buf, 1) == -1) {
		perror("semop");
		exit(1);
	}
}

void V(int semid)  /* Semaphor freigeben */
{
	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_op=1;
	buf.sem_flg=0;
	if(semop(semid, &buf, 1) == -1) {
		perror("semop");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	int semid;
	semid = InitSema(argv[0], 1);
	printf("Bin ich nun drin oder was ?!?\n");
	P(semid);
	printf("Ich bin drin :-)\n");
	getchar();
	V(semid);
        return EXIT_SUCCESS;
}
 

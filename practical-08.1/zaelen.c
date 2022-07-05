#include <stdlib.h>
#include <stdio.h> /* printf, perror */

#include <unistd.h> /* fork, getpid, getppid */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* waitpid */


/* Praktikum 8 Aufgabe 1:
	Programm erstellt Kindprozess und schaut, 
	welcher der beiden als erstes die Berechnung
	innerhalb des Programms abschließt.
*/
int main(int argc, char** argv)
{
	int fraktal = 0;
	if(argc >= 2) {	
		fraktal = atoi(argv[1])+1;
	}
	
	if(fraktal>0)
	printf("Fraktalberechnung zugeschalten\n");
	
	int status;
	/*processId*/
   pid_t processId, parentProcessId, childProcessId, waitProcessId;
   

   childProcessId = fork();

	/*Falls Fork nicht geklappt*/
   if(childProcessId < 0)
   {
       perror("Fork fehlgeschlagen: ");
       return EXIT_FAILURE;
   }

   processId = getpid();
   parentProcessId = getppid();

   printf("Proces-ID= %d, Parent Process-ID= %d, Child Process-ID= %d\n", 
   processId, parentProcessId, childProcessId);

   for(long i = 0; i < 1000000000; i++ )
   {
   	if(fraktal>0){
      /* Zusatz: Fraktalberechnung */
      	int z = fraktal+2, c = fraktal+5;
      	
      	for(int i = 0; i < 10; i++)
      	{
      		z = z * z + c;
      	}
    	}
   }

	/*Wenn Elternprozess*/
   if( childProcessId > 0)
   {
       /* WNOHANG => Sofortiges zurückkehren zum Terminal und kein
       weiteres warten auf noch nicht beendeten Prozess*/
       waitProcessId = waitpid(childProcessId, &status, WNOHANG );
       if( waitProcessId > 0 ) /* Kindprozess ist schon fertig*/
       {
           printf("Der Kindprozess war eher fertig\n");
       }
       else if( waitProcessId == 0 ) /* Kind berechnet noch*/
       {
           printf("Der Elternprozess war eher fertig\n");
       }
       else /* Fehler */
       {
           perror("waitpid");
           return EXIT_FAILURE;
       }
   }

   return EXIT_SUCCESS;
}
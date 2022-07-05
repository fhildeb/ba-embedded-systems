#include <stdlib.h>
#include <stdio.h> /* printf, perror */

#include <unistd.h> /* fork, getppid, sleep */
#include <sys/types.h> /* pid_t */

/* Praktikum 8 Aufgabe 2:
	Programm erstellt Kindprozess und beendet sich
	bevor der Kindprozess die Parent-PID ausgibt
*/
int main(int argc, char** argv)
{
   pid_t childProcessId, parentProcessId;
    
   childProcessId = fork();

   if( childProcessId > 0 ) /*wenn Elternprozess*/
   {
   	sleep(2);
		printf("Elternprozess beendet\n");
   }
   else if (childProcessId == 0) /*wenn Kindprozess*/
   {
    	
      parentProcessId = getppid();
      printf("Parent-PID= %d\n", parentProcessId);
      sleep(4);
      parentProcessId = getppid();
      printf("\nParent-PID= %d\n", parentProcessId);
      printf("Kindprozess beendet\n");
   }
   else
   {
       perror("fork");
       return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
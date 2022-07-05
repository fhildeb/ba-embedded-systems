#include <sys/types.h>
#include <sys/wait.h>
/* Beispiel 3: 	Lesen und Schreiben aus einer Pipe,
		echte Kommunikation zwischen zwei Prozessen
		Kindprozess�
		Daten werden von p3read aus stdin gelesen
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
	int fd, nread;
	char s[100];
	
	nread = read(0, s, sizeof(s));
	switch (nread) {
	   case -1:	perror("read");
	   		return EXIT_FAILURE;

	   case  0:     fprintf(stderr, "reading EOF or empty pipe\n");
	   		return EXIT_FAILURE;
	   		
	   default:	printf("read %d bytes from pipe: %s \n", nread, s);
	}
	return EXIT_SUCCESS;
}





		
		




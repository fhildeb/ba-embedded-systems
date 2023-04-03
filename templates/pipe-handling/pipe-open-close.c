#include <sys/types.h>
#include <sys/wait.h>
/* Nutzung von popen() und pclose() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char line[256];
	char *erg;

	FILE *pipe = popen("ps -aux", "r");
	int len = sizeof(line);
	if (pipe == NULL) {
		perror("popen");
		return EXIT_FAILURE;
	}

	for(;;) {
	   erg = fgets(line, sizeof(line), pipe);
	   if(erg == NULL) break;
	   /* hier Verarbeitung der Zeile */
	   puts(line);
	}
	pclose(pipe);
	return EXIT_SUCCESS;

}

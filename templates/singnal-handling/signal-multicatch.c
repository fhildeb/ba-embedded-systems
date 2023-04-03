#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FGBLACK 30
#define FGRED 31
#define FGGREEN 32
#define FGYELLOW 33
#define FGBLUE 34
#define FGMAGENTA 35
#define FGCYAN 36
#define FGWHITE 37

#define BGBLACK 40
#define BGRED 41
#define BGGREEN 42
#define BGYELLOW 43
#define BGBLUE 44
#define BGMAGENTA 45
#define BGCYAN 46
#define BGWHITE 47

#define BOLD 1
#define NOBOLD 3
#define OFF 0


void ANSIattrib(int fg,int bg,int bold)
{
	printf("\033[%d;%d;%dm",fg,bg,bold);
	fflush(stdout);
}

void ANSIattribOff()
{
	printf("\033[0m");
	fflush(stdout);
}


void exitproc(void)
{
	fprintf(stderr, "...räume auf!\n");
	ANSIattribOff();
}


void sighandler(int signo)
{
	exit(EXIT_FAILURE);
}

int main (int argc, char *argv[])
{
	struct sigaction newsa, oldsa;
	int i;

	atexit(exitproc);
	newsa.sa_handler=sighandler;
	newsa.sa_flags=0;
	sigemptyset(&newsa.sa_mask);

	for(i=1; i<16; i++) {
		if(i!=9) {
			if(sigaction(i, &newsa, &oldsa)<0) {
		   		fprintf(stderr,"Signal: %d", i);
		   		perror("sigaction");
			}
		}
	}


	/*Umschalten Ausgabefarben mit ANSI-Steuerfolgen */
	ANSIattrib(FGYELLOW, BGBLUE, BOLD);
	printf("Farben neu gesetzt!\n");
	sleep(5);
	
	return EXIT_SUCCESS;
}


#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <termios.h>

struct termios old, new;

/*****************************************************/
/*   Signalhandler   -   Bei Abbruch Terminalreset   */
/*****************************************************/
void sig_handle (int signo)
{
   tcsetattr(STDIN_FILENO, TCSANOW, &old);
   fprintf(stderr, "Abbruch durch Signal : %d\n", signo);
   exit (0);
}

/*****************************************************/
/*   tty_raw   -   Terminal in RAW-Mode setzen       */
/*****************************************************/
int tty_raw (void)
{
   signal (SIGINT, sig_handle);
   signal (SIGQUIT, sig_handle);
   if (tcgetattr(STDIN_FILENO, &old) != 0)
      return (-1);
   new = old;
   new.c_lflag &= ~ICANON;      /* raw-mode          */
   new.c_lflag &= ~ECHO;        /* Echo aus          */
   new.c_cc[VMIN] = 1;          /* Einzelzeichen     */
   new.c_cc[VTIME] = 0;         /* nicht warten      */
   return (tcsetattr(STDIN_FILENO, TCSANOW, &new));
}

/*****************************************************/
/*   restore_tty   -   Terminal in alten Zustand     */
/*****************************************************/
int restore_tty (void)
{
   return (tcsetattr(STDIN_FILENO, TCSANOW, &old));
}


int main(void)
{
    int flags;
    int erg;
    char ch;

    if ((flags = fcntl(STDIN_FILENO, F_GETFL)) < 0) {
         perror("fcntl_get");
         exit(EXIT_FAILURE);
    }

    flags = flags | O_NONBLOCK;

    if (fcntl(STDIN_FILENO, F_SETFL, flags) < 0) {
         perror("fcntl_set");
         exit(EXIT_FAILURE);
    }

    tty_raw();

    do {
        erg = read(STDIN_FILENO, &ch, sizeof(char));
        if  ((erg < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                putchar('*');
                fflush(stdout);
                sleep(1);
        } else if (erg < 0) {
		perror("read");
		exit(EXIT_FAILURE);
        } else {
                putchar(ch);  
                fflush(stdout);
        }
    } while (ch != 'E');


    flags = fcntl(STDIN_FILENO, F_GETFL);
    flags = flags & ~O_NONBLOCK;

    if (fcntl(STDIN_FILENO, F_SETFL, flags) < 0) {
         perror("fcntl");
         exit(EXIT_FAILURE);
    }

    puts("\nblockiere wieder!");
    restore_tty();    
    return EXIT_SUCCESS;
} 

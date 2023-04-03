#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fi1fd, fi2fd, fi3fd;
    int res, end=0;
    fd_set fdsin, fds;
    struct timeval timeout = {1,0}, tout;
    char ch = ' ';

    if (argc != 4) {
        fprintf(stderr, "usage: %s <in1> <in2> <in3>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Öffne alle drei übergebenen FIFOs */
    if ((fi1fd = open(argv[1], O_RDONLY|O_NONBLOCK)) < 0) {
        perror("fifo1 ");
        return EXIT_FAILURE;
    }

    if ((fi2fd = open(argv[2], O_RDONLY|O_NONBLOCK)) < 0) {
        perror("fifo2 ");
        return EXIT_FAILURE;
    }

    if ((fi3fd = open(argv[3], O_RDONLY|O_NONBLOCK)) < 0) {
        perror("fifo3 ");
        return EXIT_FAILURE;
    }

    /* melde alle drei Filedeskritporen im Set an */
    FD_ZERO(&fdsin);
    FD_SET(fi1fd, &fdsin);
    FD_SET(fi2fd, &fdsin);
    FD_SET(fi3fd, &fdsin);

    do {
        /* Warte 'timeout' Zeit auf Input an einem der drei Kanäle */
        fds = fdsin;
        tout = timeout;
        res = select(fi3fd+1, &fds, NULL, NULL, &tout);

        if (res == 0) {
            puts("+++ timeout, keine Daten +++");

        } else if (res < 0 ) {
            perror("select ");
            return EXIT_FAILURE;

        } else {
        //---------------------------Fifo1-----------------------------
            if (FD_ISSET(fi1fd, &fds)) {        /* wenn Daten an fifo1 */

                puts("Fifo1:");
                while(read(fi1fd, &ch, 1)==1) {  /* alle Daten lesen */
                    putchar(ch);
                    if(ch=='E') end=1;
                }
            }

        //---------------------------Fifo2-----------------------------
            if (FD_ISSET(fi2fd, &fds)) {

                puts("Fifo2:");
                while(read(fi2fd, &ch, 1)==1) {
                    putchar(ch);
                    if(ch=='E') end=1;
                }
            }

        //---------------------------Fifo3-----------------------------
            if (FD_ISSET(fi3fd, &fds)) {

                puts("Fifo3:");
                while(read(fi3fd, &ch, 1)==1) {
                    putchar(ch);
                    if(ch=='E') end=1;
                }
            }

        }  // else
    } while(end != 1);

    close(fi1fd);
    close(fi2fd);
    close(fi3fd);

    return EXIT_SUCCESS;
}

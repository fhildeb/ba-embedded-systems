#include <stdlib.h>
#include <stdio.h>

/* msg includes */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* datei includes */
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <string.h> /* strncpy */

#include "common.h"

int msgQueue;
struct msgbuf msg;

void checkServerResponse()
{
    /* warte auf Server antwort */
    if( msgrcv( msgQueue, &msg, MSGSIZE, 0, 0 ) < 0 )
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    
    if( msg.type == MSGT_ERR )
    {
        fprintf(stderr, "%s\n", msg.text);
        exit(EXIT_FAILURE);
    }

}

int main(int argc, char** argv)
{
    if( argc != 4 )
    {
        printf("Nutzung:\n");
        printf("\t%s SERVERNAME QUELLDATEI ZIELDATEI\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* ziel und quelldatei pfade */
    char* ftok_path = argv[1];
    char* src_path = argv[2];
    char* dst_path = argv[3];

    int src_fd;
    key_t token;
    struct stat src_stat;

    /* lies Daten aus Quelldatei */
    if( (src_fd = open( src_path, 0, O_RDONLY )) < 0)
    {
        perror("Quelldatei öffnen");
        exit(EXIT_FAILURE);
    }

    /* öffne Msg Queue */
    if( (token = ftok(ftok_path, 1)) < 0)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    if( (msgQueue = msgget(token, 0660)) < 0 )
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }


    /* sende Server den Zielname */
    msg.type = MSGT_NAM;
    strncpy( msg.text, dst_path, MSGSIZE );

    if( msgsnd( msgQueue, &msg, MSGSIZE, 0 ) < 0 )
    {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    /* falls kein fehler weiter */
    checkServerResponse();

    if(fstat(src_fd, &src_stat) < 0)
    {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    /* sende länge */
    msg.type = MSGT_LEN;
    snprintf( msg.text, MSGSIZE, "%ld", src_stat.st_size );
        
    if( msgsnd(msgQueue,  &msg, MSGSIZE, 0) < 0)
    {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    checkServerResponse();

    int bytesRead = 0;
    msg.type = MSGT_BLK;
    while( (bytesRead = read(src_fd, &msg.text, BLKSIZE)) > 0 )
    {
        if( msgsnd( msgQueue, &msg, bytesRead, 0 ) < 0 )
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    msg.type = MSGT_BYE;
    if( msgsnd( msgQueue, &msg, 0, 0 ) < 0 )
    {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
   
    return EXIT_SUCCESS;    
}
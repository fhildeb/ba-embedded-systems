#include <stdlib.h>
#include <stdio.h>

/* msgqueue includes*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* file includes*/
#include <unistd.h>
#include <fcntl.h>

/*Funktionen sigaction, ... */
#include <signal.h> 
/*Funktion errno */
#include <errno.h> 
/*Funktion strerror */
#include <string.h> 

#include "common.h"

/* globale variable MsgQueue ID (global wegen exithandler) */
int msgQueue;

void exithandler()
{
    /* lösche Message Queue */
    /* drittes argument bei RMID ignoriert */
    /* setzt errno falls bereits gelöscht, ist aber egal,
    läuft sowieso am Ende des Programms */
    printf("deleting msg queue...");
    msgctl( msgQueue, IPC_RMID, NULL );
    printf("done\n");
}

void signalhandler(int signal)
{
    exithandler();
    _exit(EXIT_SUCCESS); /* beende mit Success (da gewollt) */
}

void sendErrorToClient()
{
    
    printf("sending error to client\n");

    struct msgbuf data;
    data.type = MSGT_ERR;

    snprintf(data.text, MSGSIZE, "%s", strerror(errno)); 

    if( msgsnd(msgQueue, &data, MSGSIZE, 0) < 0)
    {
        perror("Message Send");
        exit(EXIT_FAILURE);
    }
}

void sendSuccessToClient()
{
    printf("sending success to client\n");

    struct msgbuf data;
    data.type = MSGT_SUC;

    if( msgsnd(msgQueue, &data, 0, 0) < 0 )
    {
        perror("Message Send");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv)
{
    /* installiere Signalhandler für SIGINT */
    struct sigaction handler;
    handler.sa_handler = signalhandler;
    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);

    if( sigaction(SIGINT, &handler, NULL) < 0 )
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* installiere Exithandler für Fehler */
    if(atexit(exithandler) != 0)
    {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    /* öffne Message Queue */
    key_t token;

    if( (token = ftok(argv[0], 1)) < 0)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    printf("token created\n");

    /* IPC_CREAT => erstelle FALLS nicht existiert, SONST öffne */
    if( (msgQueue = msgget(token, IPC_CREAT | 0660)) < 0)
    {
        perror("msgQueue");
        exit(EXIT_FAILURE);
    }

    printf("msgqueue created\n");

    struct msgbuf msg;  /* nachrichten daten */
    int msglen = 0;     /* tatsächliche länge */

    int fd;
    int file_size;
    ssize_t bytesWritten;

    /* Hauptroutine */
    while( 1 )
    {
        printf("waiting for client message...\n");
        /* warte auf client nachricht */
        /* id, daten, länge, typ, flags */
        if( (msglen = msgrcv(msgQueue, &msg, MSGSIZE, 0, 0)) < 0)
        {
            perror("Message Receive");
            exit(EXIT_FAILURE);
        }

        /* unterscheide nachrichtentyp "name", "länge", "datenblock" und "ende" */
        switch( msg.type )
        {
            case MSGT_NAM:
                printf("got message of type name\n");
                if( (fd = open( msg.text, O_CREAT | O_EXCL | O_WRONLY, 0644 )) < 0 )
                {
                    sendErrorToClient();
                }
                else 
                {
                    sendSuccessToClient();
                }
                break;
            case MSGT_LEN:
                printf("got message of type length\n");
                file_size = atoi(msg.text);
                if(file_size == 0)
                {
                    sendErrorToClient();
                }
                else if( ftruncate(fd, file_size) < 0 )
                {
                    sendErrorToClient();
                }                
                else
                {
                    sendSuccessToClient();
                }
                break;
            case MSGT_BLK:
                printf("got message of type block\n");
                /* schreibe nicht MSGSIZE, sondern tatsächliche länge.
                weil letzter Block mglw kürzer. */
                bytesWritten = write(fd, msg.text, msglen);
                if( bytesWritten <= 0 || bytesWritten != msglen )
                {
                    sendErrorToClient();
                }
                /*else
                {
                    sendSuccessToClient();
                }*/
                break;
            case MSGT_BYE:
                printf("got BYE\n");
                close(fd);
                break;
            default: 
                break;
        }
    }

    return EXIT_SUCCESS;    
}
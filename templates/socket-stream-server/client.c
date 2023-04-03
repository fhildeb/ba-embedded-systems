/* Client für Nummernserver - holt bei Server eine eindeutige Nummer
 *
 * - realisiert als verbundener Client, 
 * - empfängt nächste Nummer in Telegramm vom Sender
 *   Trigger per leerem Telegramm nicht nötig, da Server eintreffende 
 *   Verbindung durch accept() erfährt
 *
 * Usage: client   startet Client und liest 32-Bit Nummer
 */
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0);

int main (int argc, char **argv) {
    int my_socket;
    struct sockaddr_un server_addr;
    char buf[100];
    int numBytes;

    /* Zieladresse des Servers vorbereiten */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = PF_LOCAL;
    strncpy(server_addr.sun_path, NUMBERS, sizeof(server_addr.sun_path) - 1);

    /* Socket anlegen */
    my_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (my_socket == -1)
   	handle_error("socket");

    /* mit Server verbinden */
    if (connect(my_socket, (struct sockaddr *)&server_addr, 
		sizeof(struct sockaddr_un)) == -1)
	handle_error("connect");
 
    /* lies Antwort vom Server */
    numBytes = read(my_socket, buf, sizeof(buf));
    if (numBytes == -1) 
	handle_error("recvfrom");
    
    printf(buf, "%d\n", buf);

    /* socket schließen */
    close(my_socket);

    return EXIT_SUCCESS;
}

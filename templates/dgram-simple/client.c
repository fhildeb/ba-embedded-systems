/* Client für Nummernserver - holt bei Server eine eindeutige Nummer
 *
 * - realisiert als verbindungsloser Client, 
 * - sendet leeres Telegramm und liest nächste Nummer.
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
    struct sockaddr_un server_addr, my_addr;
    socklen_t len;
    char buf[100];
    int numBytes;

    /* Zieladresse des Servers vorbereiten */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = PF_LOCAL;
    strncpy(server_addr.sun_path, NUMBERS, sizeof(server_addr.sun_path) - 1);

    /* Socket anlegen */
    my_socket = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (my_socket == -1)
   	handle_error("socket");

    /* privaten Absendername vorbereiten und an Socket binden */
    memset(&my_addr, 0, sizeof(struct sockaddr_un));
    my_addr.sun_family = PF_LOCAL;
    snprintf(my_addr.sun_path, sizeof(my_addr.sun_path), 
	    "/tmp/client-%d", getpid());

    if (bind(my_socket, (struct sockaddr *)&my_addr, 
			    sizeof(struct sockaddr_un)) == -1)
    	handle_error("bind");

    /* leeres Packet als Trigger an Serveradresse senden */
    len = sizeof(struct sockaddr_un);
    numBytes = sendto(my_socket, buf, 0, 0, 
		    (struct sockaddr *)&server_addr, len);
    if (numBytes != 0) {
	remove(my_addr.sun_path);
	handle_error("sendto");
    }

    /* empfange antwort, ignoriere Absender-Information */
    numBytes = recvfrom(my_socket, buf, sizeof(buf), 0, NULL, NULL);
    if (numBytes == -1) {
	remove(my_addr.sun_path);
	handle_error("recvfrom");
    }

    printf(buf, "%d\n", buf);

    close(my_socket);
    remove(my_addr.sun_path);

    return EXIT_SUCCESS;
}

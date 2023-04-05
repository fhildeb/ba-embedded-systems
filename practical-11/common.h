#include <errno.h> // errno für Angabe von Server
#include <fcntl.h>
#include <limits.h> // PATH_MAX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Socket Header
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h> // sockaddr_un

#define SOCKET "/tmp/mysocket"
#define PACKAGE_SIZE 512

/*	Aufzählung aller "Handshake"-Stati numeriert von 0-5
	0 == Zieldatei
 * erstellt bzw. Kopiervorgang beendet
	1 == Fehler beim Kopieren aufgetreten

 * 2 == Auf neue Dateiinfo zum Kopieren warten
	3 == Datenpacket wurde von
 * Client gesendet
	4 == Verbindung trennen
*/
enum
{
  TELL_SUCCESS,
  TELL_ERROR,
  TELL_INFO,
  TELL_DATA_PACKAGE,
  TELL_BYE
};

/* File-Informationen die Server vom Client benötigt*/
struct fileInformation
{
  char name[PATH_MAX];
  size_t size;
};
/* Status-Informationen die der Client vom Server benötigt*/
struct serverAnswer
{
  int type;
  int error;
};
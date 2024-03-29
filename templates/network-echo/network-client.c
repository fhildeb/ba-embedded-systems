/*
 * Client baut Verbindung auf, sendet Textzeilen
 * Server setzt nach Großbuchstaben um
 * Client zeigt umgesetzte Zeile an.
 */
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* wegen bcopy */
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 56666

extern int errno;

int main(int argc, char *argv[])
{
  char line[100], *presult;
  FILE *sfp;
  int s, result;
  struct hostent *hp;
  struct sockaddr_in sin;

  /* Argumentzahl abtesten */
  if (argc != 2)
  {
    fprintf(stderr, "Nutzung: network-client <server>\n");
    return EXIT_FAILURE;
  }

  /* Adresse des Servers bestimmen */
  if ((hp = gethostbyname(argv[1])) == NULL)
  {
    fprintf(stderr, "%s: unknown host.\n", argv[1]);
    return EXIT_FAILURE;
  }

  /* Streamsocket anfordern */
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("client: socket");
    return EXIT_FAILURE;
  }

  /* Serveradresse vorbereiten */
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);
  bcopy(hp->h_addr, &sin.sin_addr.s_addr, hp->h_length);

  /* Verbindung zum Server aufbauen
   * Server muss bereits listen() aufgerufen haben! */
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    perror("client: connect");
    return EXIT_FAILURE;
  }

  /* lesen und schreiben mit gepufferter E/A */
  sfp = fdopen(s, "r+");

  /* Der Server schickt uns zwei Zeilen mit Begrüßung und
   *  Anleitung  */
  presult = fgets(line, sizeof(line), sfp); /* Zeile aus Socket lesen */
  assert(presult != NULL);
  puts(line); /* anzeigen */

  presult = fgets(line, sizeof(line), sfp); /* Zeile aus Socket lesen */
  assert(presult != NULL);
  puts(line); /* anzeigen */

  presult = fgets(line, sizeof(line), stdin); /* Zeile aus stdin lesen */
  assert(presult != NULL);

  while (strcmp(line, "ENDE\n") != 0)
  {                            /* solange nicht ENDE\n  */
    result = fputs(line, sfp); /* zum Server senden     */
    assert(result >= 0);

    presult = fgets(line, sizeof(line), sfp); /* Antwort vom Server lesen */
    assert(presult != NULL);
    puts(line); /* und anzeigen */

    presult =
        fgets(line, sizeof(line), stdin); /* nächste Zeile aus stdin lesen */
    assert(presult != NULL);
  }

  fclose(sfp); /* Socket schließen */

  return 0;
}

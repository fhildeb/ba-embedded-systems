/*
Client-Programm zum Kopieren einer Datei via
UNIX-Sockets
*/
#include "common.h"
#include <sys/stat.h>

int socketFileDescriptor;

/*
Wenn Client beendet wird: Server kontaktieren
und Verbindung abbrechen
*/
void beendeClient(void)
{
  int conversation = TELL_BYE;
  /*
  schreibts int-Bytes aus Puffer (Conversation) in die
  Datei die dem FD zugeordnet ist
  */
  write(socketFileDescriptor, &conversation, sizeof(int));
  // gibt geschriebene Bytes zurück
}

int main(int argc, char **argv)
{
  // Falls keine 2 zusätzlichen Parameter angegeben
  if (argc != 3)
  {
    printf("Programm muss mit folgendem Schema aufgerufen werden: "
           "\n ./client <Quelldatei> <Zieldatei> \n");
    exit(EXIT_FAILURE);
  }

  char *source = argv[1];
  char *destination = argv[2];

  // Filedescriptor für Quelle
  int sourceFileDescriptor;

  // Vordefiniert für Arbeit mit UNIX-Sockets
  struct sockaddr_un address;

  // Selbst erstellte Structs für Kommunikation
  struct fileInformation fileinfo;
  struct serverAnswer response;

  // Abspeichern der Quelldateiattribute auf Adresse sourceStat
  struct stat sourceStat;

  /*
  Int wert zur Überprüfung der Konversation
  sowie der Stati auf der Kommandozentrale
  */
  int conversation, running;
  int bytesWritten;

  // Quelldatei öffnen ->FD zeigt auf offene Datei
  sourceFileDescriptor = open(source, O_RDONLY);

  if (sourceFileDescriptor < 0)
  {
    perror("Quelldatei öffnen");
    exit(EXIT_FAILURE);
  }

  // Status
  printf("--Status: Quelldatei gefunden \n");

  /*
  Socket anfordern -> FD zeigt auf offenen Socket
  SOCK_STREAM -> Austausch über Stram statt Datagramm etc.
  Protokoll: unspezifiziert (0)
  */
  socketFileDescriptor = socket(AF_LOCAL, SOCK_STREAM, 0);

  // Socket nicht geöffnet
  if (socketFileDescriptor < 0)
  {
    perror("Socket anfordern");
    exit(EXIT_FAILURE);
  }

  // Status
  printf("--Status: Socket geöffnet \n");

  /*
  Socketadresse vorbereiten:
       Addresse mit null überschreiben,
       Als lokalen Socket definieren,
       Zu verwendende Socket-Addresse in address kopieren
  */
  memset(&address, 0, sizeof(address));
  address.sun_family = PF_LOCAL;
  strncpy(address.sun_path, SOCKET, sizeof(address.sun_path) - 1);

  /*
  Mit Socket zum Server verbinden:
      Pointer zur Strucktur-Adresse wo Verknüpfung definiert
      ist samt dessen Größe übergeben
      */
  if (connect(socketFileDescriptor, (struct sockaddr *)&address,
              sizeof(address)) < 0)
  {

    // Status
    fprintf(stdout, "--Status: Server nicht verbunden\n");
    fprintf(stdout, "--Status: Vorgang abgebrochen\n");
    exit(EXIT_FAILURE);
  }

  // Status
  printf("--Status: Socket verbunden \n");

  // Falls Client beendet: Beim Server abmelden
  atexit(beendeClient);

  // Dateiinformationen der Zieldatei überreichen
  strncpy(fileinfo.name, destination, sizeof(fileinfo.name) - 1);

  // Größe der Quelldatei ermitteln und für Zieldatei übergeben
  if (fstat(sourceFileDescriptor, &sourceStat) < 0)
  {

    // Status
    perror("Status Quelldatei");
    exit(EXIT_FAILURE);
  }
  fileinfo.size = sourceStat.st_size;

  /*
  Nachrichtentyp auf INFO setzen:
       Es wird neue Kopieranfrage mit
       Datei-Info übergeben
       */
  conversation = TELL_INFO;

  // Nachrichtentyp an Socket senden
  if (write(socketFileDescriptor, &conversation, sizeof(int)) < 0)
  {
    perror("Send Conversation Type");
    exit(EXIT_FAILURE);
  }

  // Datei-Info an Socket senden
  bytesWritten = write(socketFileDescriptor, &fileinfo, sizeof(fileinfo));

  if (bytesWritten < 0)
  {
    perror("Send Data Information");
    exit(EXIT_FAILURE);
  }

  // Antwort vom Server lesen
  if (read(socketFileDescriptor, &response, sizeof(struct serverAnswer)) < 0)
  {
    perror("Read Server Response");
    exit(EXIT_FAILURE);
  }

  // Falls Server Fehler festgestellt hat
  if (response.type == TELL_ERROR)
  {
    // Status
    fprintf(stderr, "--Status: Server sendet Fehler: %s\n",
            strerror(response.error));
  }

  /*
          Falls keine Fehler: Quelldatei
          in Datenpacketen vordefinierter Größe senden
  */
  char data[PACKAGE_SIZE];
  int bytesRead;
  do
  {
    // Lese PACKEGE_SIZE Bytes von der Adresse der Quelldatei
    bytesRead = read(sourceFileDescriptor, data, PACKAGE_SIZE);

    // Falls keine Daten gelesen
    if (bytesRead < 0)
    {
      perror("Read Data Package");
      exit(EXIT_FAILURE);
    }
    // Falls Dateiende erreicht
    if (bytesRead == 0)
      break;

    /*
            Conversations-Typ abändern falls wirklich
            noch Daten übertragen werden
    */
    conversation = TELL_DATA_PACKAGE;

    if (write(socketFileDescriptor, &conversation, sizeof(int)) < 0)
    {
      perror("Change Conversation Type");
      exit(EXIT_FAILURE);
    }
    // geleschriebene Bytes anpassen und Datenpacket übergeben
    bytesWritten = write(socketFileDescriptor, &data, bytesRead);

    // Falls keine Daten übergeben werden konnten
    if (bytesWritten < 0)
    {
      perror("Write Data Package");
      exit(EXIT_FAILURE);
    }

    // Antwort vom Server lesen
    if (read(socketFileDescriptor, &response, sizeof(struct serverAnswer)) < 0)
    {
      perror("Read Server Response");
      exit(EXIT_FAILURE);
    }

    // Fehler beim Kopieren
    if (response.type == TELL_ERROR)
    {
      // Status
      fprintf(stderr, "--Status: Server sendet Fehler: %s\n",
              strerror(response.error));
    }

    /*
            Falls erstes Datenpacket vom Server in
        Zieldatei geschrieben
    */
    if (response.type == TELL_SUCCESS)
    {
      // Statusausgabe auf eine Zeile begrenzen
      if (running == 0)
        fprintf(stderr, "--Status: Kopiervorgang startet\n");
      running = 1;
    }

  } while (bytesRead > 0);
  // Falls Dateiende erreicht
  conversation = TELL_BYE;

  if (write(socketFileDescriptor, &conversation, sizeof(int)) < 0)
  {
    perror("End Conversation");
    exit(EXIT_FAILURE);
  }

  // Status
  printf("--Status: Kopiervorgang abgeschlossen \n");
  return EXIT_SUCCESS;
}
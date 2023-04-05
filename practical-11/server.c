/*
    Server-Programm zum Kopieren einer Datei via
    UNIX-Sockets
*/

#include "common.h"
#include <signal.h>

int socketFileDescriptor;

// Socket schließen
void closeSocket(void) { close(socketFileDescriptor); }

// Socket löschen
void unlinkSocket(void) { unlink(SOCKET); }

// Sendet eine Conversations-Antwort and den Clienten
void sendResponse(int type, int socket)
{
  static struct serverAnswer response;

  response.type = type;

  // Auslesen aus errno.h
  response.error = errno;

  if (write(socket, &response, sizeof(struct serverAnswer)) < 0)
  {
    perror("Write Client Response");
    exit(EXIT_FAILURE);
  }
}

// Falls Signal-Interrupt, Server beenden
void beendeServer(int signal) { exit(EXIT_SUCCESS); }

int main(int argc, char **argv)
{
  // Socket
  int clientSocket;
  struct sockaddr_un address;

  // Zieldatei

  int destinationFileDescriptor;
  char destinationData[PACKAGE_SIZE];
  struct fileInformation destinationFileInfo;
  static int packagenumber = 0;

  // Interrupt
  struct sigaction sigInterrupt;

  // Kopiervorgang
  int bytesRead, bytesWritten;
  int conversation;

  /*
      Signalhandler definieren für Signalactions:
          Leeres Signal-Set initialisieren, keine Signale
          die innerhalb der Maske blockiert werden
  */
  sigemptyset(&sigInterrupt.sa_mask);

  // Action hinzufügen
  sigInterrupt.sa_handler = beendeServer;

  // Keine Flags zum Modifizieren des Verhaltens
  sigInterrupt.sa_flags = 0;

  // SIGINT (Strg+C) zum Signalhandler hinzufügen
  if (sigaction(SIGINT, &sigInterrupt, NULL) < 0)
  {
    perror("Add Signal Action");
    exit(EXIT_FAILURE);
  }

  /*
      Socket anfordern -> FD zeigt auf offenen Socket
      SOCK_STREAM -> Austausch über Stram statt Datagramm etc.
      Protokoll: unspezifiziert (0)
  */
  socketFileDescriptor = socket(AF_LOCAL, SOCK_STREAM, 0);

  if (socketFileDescriptor < 0)
  {
    perror("Socket anfordern");
    exit(EXIT_FAILURE);
  }

  /*
      Falls Server im nächsten Schritt abbricht,
          wird der Socket geschlossen
  */
  atexit(closeSocket);

  /* scoket addresse vorbereiten */
  memset(&address, 0, sizeof(address));
  address.sun_family = PF_LOCAL;
  strncpy(address.sun_path, SOCKET, sizeof(address.sun_path) - 1);

  /* socket addresse zuweisen */
  if (bind(socketFileDescriptor, (struct sockaddr *)&address, sizeof(address)) <
      0)
  {
    perror("bind socket");
    exit(EXIT_FAILURE);
  }

  /*
      Falls Server nun abbricht,
          wird der Socket zuerst vom Server gelöst
          und dannach geschlossen (atexit's werden
          gestackt und dann von oben herab abgearbeitet)
  */
  atexit(unlinkSocket);

  /*
      Warteschlange für den Socket erstellen.
      Hier beispielsweise eine Warteschlange
      von 2 Clienten
  */
  if (listen(socketFileDescriptor, 2) == -1)
  {
    perror("Socketqueue");
  }

  while (1)
  {
    // Status
    fprintf(stdout, "--Status: Wartet auf Client...\n");

    /*
        Anfragen in der Warteliste werden angenommen
                und einem neuen Socket mit gleichem Protokoll zugewiesen
                NULL->Socketaddresse und Länge nicht benötig
    */
    clientSocket = accept(socketFileDescriptor, NULL, NULL);

    // Socketanfrage nicht erfolgreich
    if (clientSocket < 0)
    {
      perror("Socket Accept");
      exit(EXIT_FAILURE);
    }

    // Status
    fprintf(stdout, "--Status: Socket verbunden \n");

    // Client Socket auslesen
    bytesRead = read(clientSocket, &conversation, sizeof(int));

    if (bytesRead < 0)
    {
      perror("Read Conversation Type");
      exit(EXIT_FAILURE);
    }
    /*
        Falls Konversations Typ keine Datei-Info, dann ist
                Anfrage keine neue Datei zum Kopieren
                -->Socket schließen, auf weitere Anfragen warten
    */
    if (conversation != TELL_INFO)
    {
      close(clientSocket);
      continue;
      // beginnt While-Schleife neu
    }

    /*
        Empfange Dateiinformationen
            falls Konversation vom Typ INFO
    */
    bytesRead = read(clientSocket, &destinationFileInfo,
                     sizeof(struct fileInformation));

    if (bytesRead < 0)
    {
      perror("Read File Info");
      exit(EXIT_FAILURE);
    }

    /*
        Erstelle Zieldatei mit folgenden Rechten
                Benutzer: Lesen (2) + Schreiben	(4)
                Gruppe: Lesen (2)
                Andere: Lesen (2)
            */
    destinationFileDescriptor =
        open(destinationFileInfo.name, O_CREAT | O_EXCL | O_WRONLY, 0644);

    /*
        Client informieren, falls Fehler beim erstellen
        der Zieldatei
    */
    if (destinationFileDescriptor < 0)
    {
      // Status
      printf("--Status: Datei existiert bereits \n"
             "     -->| Client wird informiert\n");
      sendResponse(TELL_ERROR, clientSocket);
      close(clientSocket);

      // Status
      printf("--Status: Socket geschlossen\n\n");
      continue; // Auf weitere Anfragen reagieren
    }

    /*
                Client informieren, falls Fehler beim festlegen
                der Dateigröße
    */
    if (ftruncate(destinationFileDescriptor, destinationFileInfo.size) < 0)
    {
      // Status
      printf("--Status: Zieldatei konnte nicht bearbeitet werden\n     -->| "
             "Client wird informiert\n");
      sendResponse(TELL_ERROR, clientSocket);
      close(clientSocket);
      continue; // Auf weitere Anfragen reagieren
    }

    /*
                Client informieren, wenn Server bereit ist
                um Datenpackete zu empfangen und in Zieldatei zu schreiben
    */
    sendResponse(TELL_SUCCESS, clientSocket);

    // Status
    printf("--Status: Leere Zieldatei erstellt \n");
    printf("--Status: Schreiben von Datenpacketen beginnt \n");
    while (1)
    {
      bytesRead = read(clientSocket, &conversation, sizeof(int));

      // Keine Daten empfangen
      if (bytesRead < 0)
      {
        perror("read msg type");
        exit(EXIT_FAILURE);
      }

      // Falls alle Datenpackete vom Client gesendet
      if (conversation == TELL_BYE)
      {
        break; // Kopierschleife verlassen
      }
      // Falls Packete folgen
      else if (conversation == TELL_DATA_PACKAGE)
      {
        /*
            Packet aus Socket lesen und gelesene Bytes
            aktualisieren
        */
        bytesRead = read(clientSocket, &destinationData, PACKAGE_SIZE);

        // Fehler beim Lesen obwohl Daten fehlen
        if (bytesRead < 0)
        {
          perror("Read File Data");
          exit(EXIT_FAILURE);
        }

        // Aus Socket gelesene Daten in Zieldatei schreiben
        bytesWritten =
            write(destinationFileDescriptor, &destinationData, bytesRead);

        /*
                            lient informieren und Fehler ausgeben falls
                            in bereits geöffnete Datei nicht geschrieben werden
                            kann
        */
        if (bytesWritten < 0)
        {
          sendResponse(TELL_ERROR, clientSocket);

          // Status
          printf("--Status: Fehler beim Schreiben \n"
                 "     -->| Client wird informiert\n");
          break; // Kopierschleife verlassen
        }
        // Paketnummer hochzählen für Statusausgabe
        ++packagenumber;

        // Status
        fprintf(stdout,
                "--Status: Packet %d gesendet "
                "mit %d Byte \n",
                packagenumber, bytesRead);

        /*
                            Client informieren wenn Packet erfolgreich
                            gelesen und in Datei geschrieben sodass nächstes
                            Packet folgen oder Austausch beendet werden
                            kann
        */
        sendResponse(TELL_SUCCESS, clientSocket);
      }
    }
    /*
        Anzahl der Packete für nächsten Clienten
        wieder zurücksetzen und Status auf Server-Seite angeben
    */
    packagenumber = 0;

    // Status
    printf("--Status: Datei kopiert \n");

    /*
        Wenn alles erfolgreich, Filediscriptor der
        Zieldatei schließen und Socket schließen
    */
    close(destinationFileDescriptor);
    printf("--Status: Datei geschlossen \n");
    close(clientSocket);
    printf("--Status: Socket geschlossen \n\n");
  }

  return EXIT_SUCCESS;
}
#include "common.h"

#include <sys/stat.h>

int sock_fd;

void exithandler(void)
{
  int msgt = MSGT_BYE;
  write(sock_fd, &msgt, sizeof(int));
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Nutzung:\n");
    printf("\tclient QUELLDATEI ZIELDATEI\n");
    exit(EXIT_FAILURE);
  }

  char *src_path = argv[1];
  char *dst_path = argv[2];

  int src_fd;

  struct sockaddr_un addr = {0};
  struct file_information finfo = {0};
  struct stat src_stat = {0};
  int msgt;
  int bytesWritten;
  struct server_response response = {0};

  /* quelldatei oeffnen! */
  src_fd = open(src_path, O_RDONLY);
  handle_error(src_fd, "quelldatei oeffnen");

  /* socket oeffnen */
  sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  handle_error(sock_fd, "socket oeffnen");

  /* scoket addresse vorbereiten */

  /* memset(&addr, 0, sizeof(addr));*/ /* unnnötig, da mit = {0} initialisiert
  (C99 Standard 6.7.2.1/18) */

  addr.sun_family = PF_LOCAL;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  /* socket verbinden! */
  handle_error(connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)),
               "socket verbinden");

  /* exithandler anmelden (sagt server tschüss, damit der datei schließen
   * kann)*/
  atexit(exithandler);

  /* sende dateiinformationen */
  /* dateipfad */
  strncpy(finfo.name, dst_path, sizeof(finfo.name) - 1);

  /* dateigröße */
  handle_error(fstat(src_fd, &src_stat), "stat");
  finfo.size = src_stat.st_size;

  /* sende nachrichtentyp */
  msgt = MSGT_INFO;
  handle_error(write(sock_fd, &msgt, sizeof(int)), "write msg type");

  /* sende dateiinfo */
  bytesWritten = write(sock_fd, &finfo, sizeof(finfo));
  handle_error(bytesWritten, "write file information");

  handle_error(read(sock_fd, &response, sizeof(struct server_response)),
               "read server response to file info");

  if (response.type == MSGT_ERROR)
  {
    fprintf(stderr, "Server sendet Fehler: %s\n", strerror(response.err));
  }

  /* sende dateiblöcke */

  char data[BLOCK_SIZE];
  int bytesRead;
  do
  {
    /* lese dateiblock */
    bytesRead = read(src_fd, data, BLOCK_SIZE);
    handle_error(bytesRead, "read file block");

    if (bytesRead == 0)
      break; /* stop falls dateiende! */

    msgt = MSGT_BLOCK;
    handle_error(write(sock_fd, &msgt, sizeof(int)), "write message type");
    bytesWritten = write(sock_fd, &data, bytesRead);
    handle_error(bytesWritten, "write file block");

    handle_error(read(sock_fd, &response, sizeof(struct server_response)),
                 "read server response to block");
    if (response.type == MSGT_ERROR)
    {
      fprintf(stderr, "Server sendet Fehler: %s\n", strerror(response.err));
    }

    sleep(1);

  } while (bytesRead > 0);

  /* exithandler wird sich verabschieden */
  return EXIT_SUCCESS;
}

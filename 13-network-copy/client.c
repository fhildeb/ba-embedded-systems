#include "common.h"

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Nutzung:\n");
    printf("\tclient SERVER-IP-ADRESSE QUELLDATEI ZIELDATEI\n");
    exit(EXIT_FAILURE);
  }

  struct msg_response response = {0};
  struct stat src_stat = {0};

  /* quell und zieldatei */
  char *ip_addr = argv[1];
  char *src_path = argv[2];
  char *dst_path = argv[3];

  /* versuchen quelldatei zu öffnen */
  int src_fd = open(src_path, O_RDONLY);
  handle_error(src_fd, "quelldatei oeffnen");

  /* datei eigenschaften holen */
  handle_error(fstat(src_fd, &src_stat), "stat");

  /* versuchen socket zu öffnen */
  int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
  handle_error(sock_fd, "socket oeffnen");

  /* socket adresse vorbereiten */
  struct in_addr ip = {0};
  int result = inet_aton(ip_addr);
  if (result == 0)
  {
    fprintf(stderr, "Fehler beim umwandeln der IP Adresse (%s)", ip_addr);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr = ip;

  /* socket verbinden */
  handle_error(connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)),
               "socket verbinden");

  /* Namen senden */
  struct msg_path path = {0};
  strncpy(path.path, dst_path, sizeof(path.path));

  handle_error(write(sock_fd, &path, sizeof(path)), "namen senden");

  /* Bestätigung abwarten */
  handle_error(read(sock_fd, &response, sizeof(response)),
               "antwort auf pfad lesen");

  if (response.type == MSGT_ERR)
  {
    fprintf(stderr, "Server Fehler: %s\n", strerror(response.error));
    exit(EXIT_FAILURE);
  }

  /* Dateilänge schicken */
  int len = src_stat.st_size;

  handle_error(write(sock_fd, &len, sizeof(len)), "dateilaenge schicken");

  /* Bestätigung abwarten */
  handle_error(read(sock_fd, &response, sizeof(response)),
               "antwort auf groeße lesen");

  if (response.type == MSGT_ERR)
  {
    fprintf(stderr, "Server Fehler: %s\n", strerror(response.error));
    exit(EXIT_FAILURE);
  }

  /* Datenblöcke schicken */
  struct msg_block data = {0};
  int bytesRead;

  do
  {

    /* quelldatei lesen */
    bytesRead = read(src_fd, data.block, BLOCKSIZE);
    handle_error(bytesRead, "quelldatei lesen");

    /* stop falls dateiende! */
    if (bytesRead == 0)
      break;

    /* block senden! */
    data.type = MSGT_OK;
    handle_error(write(sock_fd, &data, sizeof(data)), "dateiblock senden");

  } while (bytesRead > 0);

  /* sende tschüss an server */
  data.type = MSGT_ERR;
  handle_error(write(sock_fd, &data, sizeof(data)), "ende senden");

  return EXIT_SUCCESS;
}

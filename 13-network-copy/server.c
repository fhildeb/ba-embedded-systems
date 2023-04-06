#include "common.h"

int main(int argc, char **argv)
{
  /* signalhanlder */

  /* socket oeffnen */

  /* addresse vorbereiten */

  /* addresse binden */

  /* exithandler installieren */

  int newsocket = -1;
  int bytesRead = -1;
  int bytesWritten = -1;
  int fileSize = -1;
  int dst_fd = -1;

  struct msg_block data = {0};
  struct msg_response response = {0};
  struct msg_path path = {0};

  while (1)
  {
    /* verbindung akzeptieren */
    newsocket = accept(sock_fd, NULL, NULL);

    /* dateiname lesen */
    bytesRead = read(newsocket, &path, sizeof(path));
    handle_error(bytesRead, "pfad lesen");

    /* datei oeffnen */
    dst_fd = open(path.path, O_CREAT | O_EXCL | O_WRONLY, 0777);

    /* antwort senden */
    if (dst_fd < 0)
    {
      response.type = MSGT_ERR;
      response.error = errno;
    }
    else
    {
      response.type = MSGT_OK;
    }

    handle_error(bytesWritten = write(newsocket, &response, sizeof(response)),
                 "antwort schicken");

    /* falls fehler, gleich weiter mit nächstem Client */
    if (response.type == MSGT_ERR)
      continue;

    /* dateilänge lesen */
    bytesRead = read(newsocket, &fileSize, sizeof(int));
    handle_error(bytesRead, "dateigroesse lesen");

    /* dateilänge kürzen */
    if (ftruncate(dst_fd, fileSize) < 0)
    {
      response.type = MSGT_ERR;
      response.error = errno;
    }
    else
    {
      response.type = MSGT_OK;
    }

    /* antwort senden */
    handle_error(bytesWritten = write(newsocket, &response, sizeof(response)),
                 "antwort schicken");

    /* falls fehler, weiter mit nächstem client */
    if (response.type == MSGT_ERR)
      continue;

    /* dateiblöcke lesen */
    do
    {
      /* block lesen */
      bytesRead = read(newsocket, &data, sizeof(data));

      /* falls ende, ende */
      if (data.type == MSGT_ERR)
        break;

      /* datenblock schreiben */
      bytesWritten = write(dst_fd, data.block, sizeof(data.block));

      if (bytesWritten < 0)
      {
        response.type = MSGT_ERR;
        response.error = errno;

        handle_error(write(newsocket, &response, sizeof(response)),
                     "antwort schreiben");
      }

    } while (data.type != MSGT_ERR);

    close(newsocket);
    close(dst_fd);
  }

  return EXIT_SUCCESS;
}

/* signal handler */
#include <signal.h>

/* threads */
#include <pthread.h>

#include "common.h"

#define handle_thread_error(val, msg)                                          \
  if ((val) < 0)                                                               \
  {                                                                            \
    perror((msg));                                                             \
    pthread_exit(NULL);                                                        \
  }

int socketfd;
pthread_attr_t thread_attr;

void close_socket(void) { close(socketfd); }

void delete_socket(void) { unlink(SOCKET_PATH); }

void delete_attr(void) { pthread_attr_destroy(&thread_attr); }

void signalhandler(int signal)
{
  /* ruft dann automatisch installierte exithandler */
  exit(EXIT_SUCCESS);
}

void send_response(int type, int socket)
{
  static struct server_response response = {0};

  response.type = type;
  response.err = errno;
  handle_thread_error(write(socket, &response, sizeof(struct server_response)),
                      "write response");
}

void pthread_exithandler(void *arg)
{
  int socket = *((int *)arg);
  close(socket);
}

void *thread_function(void *argument)
{
  /* socket = daten an adresse( argument als int-pointer ) */
  int socket = *((int *)argument);

  /* exithandler! */
  pthread_cleanup_push(pthread_exithandler, (void *)&socket);

  int bytesRead, bytesWritten, msgt;
  int filefd;
  struct file_information finfo = {0};
  char fdata[BLOCK_SIZE];

  /* falls nächste Nachricht NICHT Dateiinfo ist, nächster! */
  bytesRead = read(socket, &msgt, sizeof(int));

  handle_thread_error(bytesRead, "read msg type");

  if (msgt != MSGT_INFO)
  {
    pthread_exit(NULL);
  }

  /* Empfange Dateiinformationen */
  bytesRead = read(socket, &finfo, sizeof(struct file_information));
  handle_thread_error(bytesRead, "read file info");

  /* oeffne Datei */
  filefd = open(finfo.name, O_CREAT | O_EXCL | O_WRONLY, 0644);

  /* falls Fehler, sende Fehler an Client */
  if (filefd < 0)
  {
    send_response(MSGT_ERROR, socket);
    pthread_exit(NULL);
  }

  /* versuche größe zu setzen, falls fehler, sende fehler an client */
  if (ftruncate(filefd, finfo.size) < 0)
  {
    send_response(MSGT_ERROR, socket);
    pthread_exit(NULL);
  }

  /* falls alles gut, sende success zu client */
  send_response(MSGT_SUCCESS, socket);

  while (1)
  {
    bytesRead = read(socket, &msgt, sizeof(int));
    handle_thread_error(bytesRead, "read msg type");

    if (msgt == MSGT_BYE)
    {
      break; // loop beenden, keine weiteren daten!
    }
    else if (msgt == MSGT_BLOCK)
    {
      bytesRead = read(socket, &fdata, BLOCK_SIZE);
      handle_thread_error(bytesRead, "read file data");

      // versuche daten in datei zu schreiben
      bytesWritten = write(filefd, &fdata, bytesRead);
      if (bytesWritten < 0)
      {
        send_response(MSGT_ERROR, socket);
        break; // fehler, loop beenden!
      }
      send_response(MSGT_SUCCESS, socket);
    }
  }

  /* fertig, socket schließen */
  close(filefd);
  close(socket);

  pthread_cleanup_pop(0); /*NOTWENDIG!!*/
  return NULL;
}

int main(int argc, char **argv)
{
  /* variablen */
  int accept_socket;
  struct sockaddr_un addr = {0};
  struct sigaction new_action = {0};
  pthread_t thread_handle; /* wird nur benötigt weil sich der compiler sonst
                              beschwert */

  /* signalhandler installieren  */
  sigemptyset(&new_action.sa_mask);
  new_action.sa_handler = signalhandler;
  new_action.sa_flags = 0;

  handle_error(sigaction(SIGINT, &new_action, NULL), "sigaction");

  /* socket filedescriptor holen */
  socketfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  handle_error(socketfd, "socket oeffnen");

  /* exit handler 1 */
  atexit(close_socket);

  /* scoket addresse vorbereiten */
  /* memset(&addr, 0, sizeof(addr));*/
  addr.sun_family = PF_LOCAL;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  /* socket addresse zuweisen */
  handle_error(bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)),
               "bind socket");

  /* exithandler 2 */
  atexit(delete_socket);

  /* höre bis zu 5 clients zu */
  handle_error(listen(socketfd, 5), "listen socket");

  /* Thread Attribute vorbereiten */
  pthread_attr_init(&thread_attr);
  pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

  /* exithandler 3 */
  atexit(delete_attr);

  while (1)
  {
    /*
        unser socket, client address struct, struct größe
        (client addresse egal, deshalb null)
    */
    accept_socket = accept(socketfd, NULL, NULL);
    handle_error(accept_socket, "accept");

    /*
erstellt thread als DETACHED
        dann müssen wir nichts aufräumen!
*/
    handle_error(pthread_create(&thread_handle, &thread_attr, thread_function,
                                (void *)&accept_socket),
                 "thread starten");
  }

  return EXIT_SUCCESS;
}

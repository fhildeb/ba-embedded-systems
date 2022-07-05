#ifndef PRAK11_COMMON_H
#define PRAK11_COMMON_H

/* standard includes */
#include <stdlib.h>
#include <stdio.h>

#include <errno.h> /* errno */
#include <limits.h> /* PATH_MAX */

/* socket includes */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> /* sockaddr_un */

/* datei includes */
#include <unistd.h>
#include <fcntl.h>

/* strncpy */
#include <string.h>

#define SOCKET_PATH "/tmp/mysocket"
#define BLOCK_SIZE 1024

#define handle_error(val, msg) if((val) < 0){ perror((msg)); exit(EXIT_FAILURE); }

enum { MSGT_SUCCESS, MSGT_ERROR, MSGT_INFO, MSGT_BLOCK, MSGT_BYE };

struct file_information {
    char name[PATH_MAX];
    size_t size;
};

struct server_response {
    int type;
    int err;    
};

#endif

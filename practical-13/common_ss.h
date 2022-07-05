#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>

#include <limits.h>

#include <string.h>

#include <netinet/in.h>

#define handle_error(val, msg) if((val) < 0) { perror(msg); exit(EXIT_FAILURE); }

#define PORT 42000

#define BLOCKSIZE 1024

enum { MSGT_OK, MSGT_ERR };

struct msg_response {
    int type;  
    int error; 
};

struct msg_block {
    int type;
    char block[BLOCKSIZE];
};

struct msg_path {
    char path[PATH_MAX];
};


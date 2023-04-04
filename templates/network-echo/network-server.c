#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <errno.h>

int main()
{
	struct sockaddr_in peerin;    
	struct hostent* peernameptr;
	struct hostent unknown = {"unknown", NULL, AF_INET, 4, NULL};
    socklen_t inlen;
	int res, i;
	void *pres;
	int logfd = open("/tmp/LogF", O_WRONLY|O_APPEND|O_CREAT, 0644);
	FILE *logF = fdopen(logfd, "a");
	FILE *sockF;
	char line[80];

	inlen = sizeof(struct sockaddr_in);
	res = getpeername(0, (struct sockaddr*) &peerin, &inlen);
	if(res < 0) {
        perror("getpeername");
        return EXIT_FAILURE;
    }
	
	peernameptr = gethostbyaddr(&peerin.sin_addr, 4, AF_INET);

	// DNS konnte Peer nicht auflösen -> "unknown" verwenden
    if(peernameptr == NULL) {
        peernameptr = &unknown;
    }

	res=fprintf(logF, 
                "Connected to %s ip %s port %d\n",
                peernameptr->h_name,
                inet_ntoa(peerin.sin_addr), 
                ntohs(peerin.sin_port)
                );

	sockF = fdopen(0, "r+");
	assert(sockF != NULL);

	res=fprintf(sockF, 
                "Hallo  %s IP %s Port %d\n",
                peernameptr->h_name,
                inet_ntoa(peerin.sin_addr),
                ntohs(peerin.sin_port)
                );
	assert(res >= 0);

	res = fputs("gib Zeilen ein, Ende ist ENDE\n", sockF);
	assert(res >=0);

	while((pres=fgets(line, sizeof(line), sockF))!=NULL) {
		for(i=0; i<strlen(line)-1; i++) {
		    line[i]=toupper(line[i]);
		}
		res=fputs(line, sockF);
		assert(res >=0);
	 }

	 res = fclose(sockF);
	 assert(res >=0 );
     res = fclose(logF);
     assert(res >= 0);
	 exit(EXIT_SUCCESS);
}



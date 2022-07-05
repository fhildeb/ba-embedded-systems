/*** icmp_log.c ***/
/* Beispiel nach http://www.zotteljedi.de/doc/raw-socket-tipps.html */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <malloc.h>

#define BUFLEN 1600

char *icmp_typ(unsigned short int type, char *antwort, int antw_len)
{
	switch(type) {
		case ICMP_ECHOREPLY: strncpy(antwort, "ICMP_ECHOREPLY", antw_len); break;
		case ICMP_DEST_UNREACH: strncpy(antwort, "ICMP_DEST_UNREACH", antw_len); break;
		case ICMP_SOURCE_QUENCH: strncpy(antwort, "ICMP_SOURCE_QUENCH", antw_len); break;
		case ICMP_REDIRECT: strncpy(antwort, "ICMP_REDIRECT", antw_len); break;
		case ICMP_ECHO: strncpy(antwort, "ICMP_ECHO", antw_len); break;
		case ICMP_TIME_EXCEEDED: strncpy(antwort, "ICMP_TIME_EXCEEDED", antw_len); break;
		case ICMP_PARAMETERPROB: strncpy(antwort, "ICMP_PARAMETERPROB", antw_len); break;
		case ICMP_TIMESTAMP: strncpy(antwort, "ICMP_TIMESTAMP", antw_len); break;
		case ICMP_TIMESTAMPREPLY: strncpy(antwort, "ICMP_TIMESTAMPREPLY", antw_len); break;
		case ICMP_INFO_REQUEST: strncpy(antwort, "ICMP_INFO_REQUEST", antw_len); break;
		case ICMP_INFO_REPLY: strncpy(antwort, "ICMP_INFO_REPLY", antw_len); break;
		case ICMP_ADDRESS: strncpy(antwort, "ICMP_ADDRESS", antw_len); break;
		case ICMP_ADDRESSREPLY: strncpy(antwort, "ICMP_ADDRESSREPLY", antw_len); break;
		default: strncpy(antwort, "unbekannt", antw_len); break;
	};

	return antwort;
}

int main(int argc, char *argv[])
{
	int s, bytes;
	struct icmphdr *icmp;
	struct iphdr *ip;
	struct in_addr addr;
	char *buffer, antwort[32];

	if ((s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("socket() failed");
		return EXIT_FAILURE;
	}
		
	buffer = (char *)malloc(BUFLEN);	/* Puffer für ein Paket  */
	ip = (struct iphdr*) buffer;	/* beginnt mit IP-Header */
	icmp = (struct icmphdr*) (buffer + sizeof(struct iphdr));
					/* danach ICMP-Header 	 */

	while((bytes = recv(s, buffer, BUFLEN, 0)) > 0) {
		addr.s_addr = ip->saddr;
		printf("ICMP Paket von %s empfangen, Gesamtlänge: %i Bytes\n",
			 inet_ntoa(addr), bytes);
		printf("Typ: %s\n", icmp_typ(icmp->type, antwort, sizeof(antwort)));
		printf("Time To Live: %i\n\n", ip->ttl);
	}
	
	free(buffer); 	/* kein Systemruf, ändert errno nicht! */

	if (bytes == -1) {
		perror("recv() failed");
		return EXIT_FAILURE;
	}	

	return EXIT_SUCCESS;
}
/*** Ende von icmp_log.c ***/

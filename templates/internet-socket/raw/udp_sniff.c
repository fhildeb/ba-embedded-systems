/*** udp_info.c ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

int main(int argc, char *argv[])
{
	int s, bytes;
	char buffer[1024], *data;
	struct iphdr *ip;
	struct udphdr *udp;
	struct in_addr addr;

	s = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	if (s < 0) {
		perror("socket() failed");
		return 1;
	}

	if( (bytes = recv(s, buffer, sizeof(buffer), 0)) == -1) {
		perror("recv() failed");
		return 2;
	}

	ip = (struct iphdr*) buffer;
	udp = (struct udphdr*) (buffer + sizeof(struct iphdr));
	data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);
	buffer[bytes] = '\0';

	printf("%i Bytes empfangen\n", bytes);
	printf("Time To Live: %i\n", ip->ttl);
	addr.s_addr = ip->saddr;
	printf("Absender: %s\n", inet_ntoa(addr));
	addr.s_addr = ip->daddr;
	printf("Empfänger: %s\n", inet_ntoa(addr));
	printf("Quellport: %i\n", ntohs(udp->source));
	printf("Zielport: %i\n", ntohs(udp->dest));
	printf("Länge der Nutzlast: %li Bytes\n", ntohs(udp->len) - sizeof(struct udphdr));

	return EXIT_SUCCESS;
}


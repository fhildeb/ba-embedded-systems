/*** tcp_sniff.c ***/
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
int main(int argc, char *argv[])
{
	int s, bytes;
	char buffer[1024], *data;

	s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if (s == -1) {
		perror("socket() failed");
		return 1;
	}

	while((bytes = recv(s, buffer, sizeof(buffer), 0)) > 0) {
		data = buffer + sizeof(struct iphdr) + sizeof(struct tcphdr);
		buffer[bytes] = '\0';
		printf("%s", data);
		fflush(stdout);
	}

	return 0;
}
/*** Ende von tcp_sniff.c ***/

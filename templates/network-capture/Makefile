icmp-sniff: icmp-sniff.c
	gcc -Wall -Wextra -Wpedantic -std=c99 -o icmp-sniff icmp-sniff.c

tcp-sniff: tcp-sniff.c
	gcc -Wall -Wextra -Wpedantic -std=c99 -o tcp-sniff tcp-sniff.c

udp-sniff: udp-sniff.c
	gcc -Wall -Wextra -Wpedantic -std=c99 -o udp-sniff udp-sniff.c

.PHONY: clean
clean:
	rm -f icmp-sniff tcp-sniff udp-sniff *.o *~

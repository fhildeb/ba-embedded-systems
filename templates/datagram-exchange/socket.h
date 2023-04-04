#if !defined(__SOCKET_H)
#define __SOCKET_H
#define ORDER_SOCKET "/tmp/OrderDgram"
#define CLIENT_SOCKET "/tmp/Client_"

enum {sinus, cosinus, tangens};

char *s[] = {"Sinus", "Cosinus", "Tangens"};

struct order {
	int functno;
	double argument;
};

struct result {
	int functno;
	double argument;
	double result;
};
#endif


#if !defined(__SOCKBSP_H)
#define __SOCKBSP_H
#define ORDER_SOCKET "/tmp/OrderStream"

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


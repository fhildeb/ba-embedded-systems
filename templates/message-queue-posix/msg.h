/* Achtung! MQ-Namen müssen mit / beginnen! */
#if !defined(__MSGBSP_H)
#define __MSGBSP_H
#define ORDER_MQ "/Order"
#define RESULT_MQ "/Result_"

enum
{
  sinus,
  cosinus,
  tangens
};

char *s[] = {"Sinus", "Cosinus", "Tangens"};

struct order
{
  char res_queue[20];
  int functno;
  double argument;
};

struct result
{
  int functno;
  double argument;
  double result;
};
#endif

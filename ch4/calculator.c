#include <stdio.h>
#include <string.h>

#define END_ELEMENT 0
#define END_LINE 98
#define END_INPUT 123

#define ELEM_OPER 110
#define ELEM_NUMBER 111
#define ELEM_VAR 112

#define OPER_PLUS 11
#define OPER_MINUS 12
#define OPER_MULT 17
#define OPER_DIV 13
#define OPER_MOD 14
#define OPER_EXP 15
#define OPER_LOG 16

#define NO_VAL 1110187


int read_element(char *buf)
{
  int c;
  int i = 0;
  while (c = getchar())
    {
      if (i == 0 && (c == ' ' || c == '\t'))
        continue;
      if (i > 0 && (c == ' ' || c == '\t'))
        {
          buf[i] = 0;
          return END_ELEMENT;
        }
      if (c == EOF)
        {
          buf[i] = 0;
          return END_INPUT;
        }
      if (c == '\n')
        {
          buf[i] = 0;
          return END_LINE;
        }
      else
        buf[i++] = c;
    }
  i--;
  while (i > 0 && (buf[i] == ' ' || buf[i] == '\t'))
    i--;
  buf[i + 1] = 0;
  return END_ELEMENT;
}

char *trim(char *str)
{
  int len = strlen(str);
  char *c = str;
  while (*c == ' ' || *c == '\t' || *c == ';')
    c++;
  char *d = &str[len - 1];
  while (*d == ' ' || *d == '\t' || *d == ';')
    d--;
  *(d + 1) = 0;
  return c;
}

int is_number(char *str)
{
  while (*str >= '0' && *str <= '9')
    str++;
  return str == 0;
}

int read_number(char *str)
{
  int Result = 0;
  while (*str)
    {
      Result *= 10;
      Result += *str - '0';
    }
  return Result;
}

/* elements: numbers([0-9]+), operations(+,-,*,/,%,exp,log), variables(rest) */
void parse_element(char *elem, int *type, int *value)
{
  if (strcmp(elem, "+") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_PLUS;
    }
  else if (strcmp(elem, "-") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_MINUS;
    }
  else if (strcmp(elem, "*") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_MULT;
    }
  else if (strcmp(elem, "/") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_DIV;
    }
  else if (strcmp(elem, "%") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_MOD;
    }
  else if (strcmp(elem, "exp") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_EXP;
    }
  else if (strcmp(elem, "log") == 0)
    {
      *type = ELEM_OPER;
      *value = OPER_LOG;
    }
  else if (is_number(elem))
    {
      *type = ELEM_NUMBER;
      *value = read_number(elem);
    }
  else
    {
      *type = ELEM_VAR;
      *value = NO_VAL;
    }
}




int main(int argc, char **argv)
{
  if (argc == 1)                /* read from stdin */
    {
      int res;
      char buf[0x100];
      while ((res = read_element(buf)) != END_INPUT)
        {
          switch (res)
            {
            case END_ELEMENT:   /* work with element */
              printf("element: '%s'\n", buf);
              break;
            case END_LINE:      /* send end of line to the calculator */
              printf("end of line\n");
              break;
            case END_INPUT:     /* send end of input to the calculator */
              printf("end of input\n");
              break;
            }
        }
    }
  else                          /* read from argv */
    {
      for (int i = 1; i < argc; i++)
        {
          char *el = trim(argv[i]);
          printf("element=%s\n", el);
          /* TODO: send element to calculator */
        }
      /* TODO: send end of input to calculator */
    }
}

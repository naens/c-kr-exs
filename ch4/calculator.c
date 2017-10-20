#include <stdio.h>
#include <string.h>
#include <math.h>

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

#define STACK_SIZE 0x100

int variables[0x100];
int stack[STACK_SIZE];
int sp;
int next_element;
int read_element(char *buf)
{
  int c;
  int i = 0;
  if (next_element)
    {
      int res = next_element;
      next_element = 0;
      return res;
    }
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
          if (i > 0)
            {
              next_element = END_INPUT;
              return END_ELEMENT;
            }
          else
            return END_INPUT;
        }
      if (c == '\n')
        {
          buf[i] = 0;
          if (i > 0)
            {
              next_element = END_LINE;
              return END_ELEMENT;
            }
          else
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
  return *str == 0;
}

int read_number(char *str)
{
  int Result = 0;
  while (*str)
    {
      Result *= 10;
      Result += *str - '0';
      str++;
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

int apply(int oper, int n1, int n2)
{
  switch (oper)
    {
    case OPER_PLUS:
      return n1 + n2;
    case OPER_MINUS:
      return n2 - n1;
    case OPER_MULT:
      return n1 * n2;
    case OPER_DIV:
      return n2 / n1;
    case OPER_EXP:
      return pow(n1,n2);
    case OPER_LOG:
      /* logarithm of x base b = log(x)/log(b) */
      return log(n2) / log(n1);
    }
  return -1;
}

int get_value(char *s)
{
  return variables[*s];
}

void print_stack()
{
  for (int i = 0; i < sp; i++)
    printf("[%d]=%d\n", i, stack[i]);
}

void push(int n)
{
  stack[sp++] = n;
}

int pop()
{
  return stack[--sp];
}

void work_with(char *s, int type, int value)
{
  //printf("work_with: s=%s, type=%d, value=%d\n", s, type, value);
  int n1, n2;
  switch (type)
    {
    case ELEM_OPER:
      n1 = pop();
      n2 = pop();
      push(apply(value, n1, n2));
      printf("work with oper %d, numbers %d,%d, result %d\n", value, n1, n2, apply(value, n1, n2));
      break;
    case ELEM_NUMBER:
      printf("work with number %d\n", value);
      push(value);
      break;
    case ELEM_VAR:
      push(get_value(s));
      break;
    }
}

void calc_eol()
{
  print_stack();
  printf("[Val=%d]EOL\n", stack[sp-1]);
}

void calc_eof()
{
  int n = pop();
  printf("[Result=%d]EOF\n", n);
}

int main(int argc, char **argv)
{
  sp = 0;
  next_element = 0;
  if (argc == 1)                /* read from stdin */
    {
      int res;
      char buf[0x100];
      int type;
      int value;
      while ((res = read_element(buf)) != END_INPUT)
        {
          switch (res)
            {
            case END_ELEMENT:   /* work with element */
              parse_element(buf, &type, &value);
              work_with(buf, type, value);
              break;
            case END_LINE:      /* send end of line to the calculator */
              calc_eol();
              break;
            case END_INPUT:     /* send end of input to the calculator */
              calc_eof();
              break;
            }
        }
    }
  else                          /* read from argv */
    {
      for (int i = 1; i < argc; i++)
        {
          char *el = trim(argv[i]);
          int type;
          int value;
          parse_element(el, &type, &value);
          work_with(el, type, value);
        }
      calc_eof();
    }
}

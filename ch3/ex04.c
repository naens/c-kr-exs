#include <stdio.h>
#include <stdlib.h>

void exchange(char *pc1, char *pc2)
{
  char c = *pc1;
  *pc1 = *pc2;
  *pc2 = c;
}

void rev(char *s)
{
  int len = 0;
  while (s[len])
    len++;
  for (int i = 0; i < len / 2; i++)
    exchange(&s[i], &s[len - 1 - i]);
}

void itoa(char n, char *s)
{
  char *p = s;
  int is_negative = n < 0;
  do {
    *(p++) = (is_negative ? -(n % 10) : n % 10) + '0';
    n /= 10;
  } while (n);
  if (is_negative)
    *(p++) = '-';
  *p = 0;
  rev(s);
}

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "arg=number\n");
      return -1;
    }

  int number = atoi(argv[1]);

  printf("number is %d\n", number);

  char buf[0x100];
  itoa(number, buf);

  printf("string is \"%s\"\n", buf);

  return 0;
}

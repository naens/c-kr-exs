#include <stdio.h>
#include <stdlib.h>

int itoa_fn(int n, char *s)
{
  if (n > 0)
    {
      int len = 1;
      if (n / 10 > 0)
        len += itoa_fn(n / 10, s);
      s[len - 1] = (n % 10) + '0';
      return len;
    }
  return 0;
}

void itoa(int n, char *s)
{
  if (n < 0)
    {
      n = -n;
      *(s++) = '-';
    }
  else if (n == 0)
    {
      s[0] = '0';
      s[1] = 0;
      return;
    }
  int len = itoa_fn(n, s);
  s[len] = 0;
}

int main(int argc, char **argv)
{
  int n;
  if (argc == 2)
    n = atol(argv[1]);
  else
    n = 0;
  char buf[0x100];
  itoa(n, buf);
  printf("%s\n", buf);
  return 0;
}

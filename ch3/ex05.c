#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void revs(char *str)
{
  int l = strlen(str);
  for (int i = 0; i < l / 2; i++)
    {
      char c = str[i];
      str[i] = str[l - 1 - i];
      str[l - 1 - i] = c;
    }
}


void itob(int n, char *s, int b)
{
  int i = 0;
  int neg = n < 0;

  if (b > 36 || b < 1)
    {
      *s = 0;
      return;
    }

  do {
    int dgt = neg ? -(n % b) : (n % b);
    s[i++] = dgt + (dgt <= 9 ? '0' : 'A' - 10);
    n /= b;
  } while (n != 0);

  if (neg)
    s[i++] = '-';

  s[i] = 0;
  revs(s);
}

int main(int argc, char **argv)
{
  if (argc != 3)
    {
      fprintf(stderr, "haha! arguments must be <number> AND <base>\n");
      return 1;
    }

  int n = atoi(argv[1]);
  int base = atoi(argv[2]);

  char str[0x100];
  itob(n, str, base);
  printf("%s\n", str);

  return 0;
}

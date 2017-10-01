#include <stdio.h>
#include <stdlib.h>

void rev(char *s)
{
  int l = 0;
  while (s[l])
    l++;

  char b;
  for (int i = 0; i < l / 2; i++)
    b = s[i], s[i] = s[l - i - 1], s[l - i - 1] = b;
}

void itoa(int n, char *s, int w)
{
  int neg = n < 0;

  int i = 0;
  do {
    s[i++] = '0' + (neg ? -(n % 10) : n % 10);
    n /= 10;
  } while (n != 0);

  if (neg)
    s[i++] = '-';

  while (i < w)
    s[i++] = ' ';

  s[i] = 0;
  rev(s);
}

int main(int argc, char **argv)
{
  if (argc != 3)
    {
      fprintf(stderr, "?\n\n");
      return 42;
    }

  int n = atoi(argv[1]);
  int w = atoi(argv[2]);
  char str[0x100];
  itoa(n, str, w);
  printf("[%s]\n", str);

  return 0;
}

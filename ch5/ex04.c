#include <stdio.h>

int strend(char *s, char *t)
{
  char *s0 = s;
  char *t0 = t;
  while (*s)
    s++;
  while (*t)
    t++;
  while (s > s0 && t > t0 && *s == *t)
    s--, t--;
  return t == t0 && *t == *s;
}

int main(int argc, char **argv)
{
  if (argc < 3)
    return 0;
  printf("%d\n", strend(argv[1], argv[2]));
}

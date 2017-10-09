#include <stdio.h>

int strrindex (char *s, char *t)
{
  int i = 0;
  int res = -1;
  while (s[i])
    {
      char *tmp = &s[i];
      char *t2 = t;
      while (*tmp && *t2 && *tmp == *t2)
        tmp++, t2++;
      if (*t2 == 0)
        res = i;
      i++;
    }
  return res;
}

int main (int argc, char **argv)
{
  if (argc != 3)
    {
      fprintf(stderr, "usage %s <string> <substring>\n", argv[0]);
      return -1;
    }

  printf("%d\n", strrindex(argv[1], argv[2]));
}

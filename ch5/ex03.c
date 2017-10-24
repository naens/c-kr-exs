#include <stdio.h>

void cat(char *s, char *t)
{
  while (*s)
    s++;
  while (*s = *t)
    s++, t++;
}

int main(int argc, char **argv)
{
  if (argc < 3)
    return -1;
  char *str1 = argv[1];
  char *str2 = argv[2];
  cat(str1, str2);
  printf("%s\n", str1);
}

#include <stdio.h>
#include <string.h>

void reverse(char *s)
{
  int len = strlen(s);
  char c;
  for (int i = 0; i < len / 2; i++)
    c = s[i], s[i] = s[len - 1 - i], s[len - 1 - i] = c;
}

int main(int argc, char **argv)
{
  char *s = argc == 2 ? argv[1] : argv[0];
  reverse(s);
  printf("%s\n", s);
}

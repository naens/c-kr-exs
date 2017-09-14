#include <stdio.h>
#include <string.h>
#include <libgen.h>

int lower(int c)
{
  return c >= 'A' && c <= 'Z' ? c += 'a' - 'A' : c;
}


int main(int argc, char **argv)
{
  if (argc != 2 || argc == 2 && strlen(argv[1]) != 1)
    {
      fprintf(stderr, "usage %s <char>\n", basename(argv[0]));
      return -235;
    }

  printf("%c\n", lower(argv[1][0]));

  return 0;
}

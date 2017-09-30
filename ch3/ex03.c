#include <stdio.h>
#include <stdlib.h>

void expand(char *s1, char *s2)
{
  if (!*s1)
    return;
  char el = 0x20;
  int shorthand = 0;

  if (*s1 == '-')
    {
      *(s2++) = '-';
      s1++;
      el = '-';
    }

  while (*s1)
    {
      if (*s1 == '-' && !shorthand)
        {
          shorthand = 1;
          s1++;
        }
      else
        {
          if (shorthand)
            {
              int step = el < *s1 ? 1 : -1;
              for (char c = el + step; c != *s1; c += step)
                *(s2++) = c;
              shorthand = 0;
            }
          el = *(s2++) = *(s1++);
        }
    }
  *s2 = 0;

}

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "usage: %s <string>\n", argv[0]);
      exit(-1);
    }

  char str[0x100];
  expand(argv[1], str);
  printf("%s\n", str);
}

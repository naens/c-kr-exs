#include <stdio.h>

int contains(char *str, char c)
{
  while(*str)
    if (*(str++) == c)
      return 1;
  return 0;
}

int any(char *s1, char *s2)
{
  int i = 0;
  while(*s1)
    {
      if (contains(s2, *(s1++)))
        return i;
      i++;
    }
  return -1;
}

int main()
{
  int c;
  char s[0x100][0x100];
  int i = 0;
  int j = 0;
  int space = 1;
  while ((c = getchar()) != EOF)
    {
      if (c == '\n')
        {
          s[i][j] = 0;
          if (i == 0)
            break;
          for (int x = 0; x <= i; x++)
            printf("string %d is %s\n", x, s[x]);
          printf("%s contains any of %s at %d\n", s[0], s[1], any(s[0], s[1]));
          i = 0;
          j = 0;
          space = 1;
        }
      else if (c == ' ')
        {
          if (!space)
            {
              s[i][j] = 0;
              space = 1;
              i++;
            }
        }
      else
        {
          if (space)
            {
              space = 0;
              j = 0;
            }
          s[i][j] = c;
          j++;
        }
    }
}

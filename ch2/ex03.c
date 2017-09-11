#include <stdio.h>

char *trim(char *s)
{
  while (*s && *s != '-'
         && !(*s >= '0' && *s <= '9' || *s >= 'a' && *s <= 'f' || *s >= 'A' && *s <= 'F'))
    s++;
  return s;
}

int htoi(char *s)
{
  int result = 0;
  s = trim(s);
  int sign = *s == '-';
  while (*s)
    {
      if (*s >= '0' && *s <= '9')
        result = 0x10 * result + *s - '0';
      else if (*s >= 'a' && *s <= 'f')
        result = 0x10 * result + *s - 'a' + 0xa;
      else if (*s >= 'A' && *s <= 'F')
        result = 0x10 * result + *s - 'A' +0xA;
      s++;
    }
  return sign ? -result : result;
}

int main()
{
  char str[0x1000];
  int c;
  int i = 0;
  while ((c = getchar()) != EOF)
    {
      if (i > 0x1000)
        break;

      if (c == '\n')
        {
          str[i] = 0;
          printf("%x\n", htoi(str));
          i = 0;
        }
      else
        {
          str[i] = c;
          i++;
        }
    }
}
  

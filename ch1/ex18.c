#include <stdio.h>

#define MAXLEN 120

int readln(char *s, int max_len)
{
  char c;
  int len = 0;
  while ((c = getchar()) != EOF)
    {
      if (len >= max_len - 1)
	break;
      s[len++] = c;
      if (c == '\n')
	break;
    }
  s[len - 1] = 0;
  return len;
}

char *trimstr(char *str)
{
  /* trim beginning */
  int i = 0;
  while (str[i] && str[i] != '\n' && (str[i] == '\t' || str[i] == ' '))
    i++;

  char *res = &str[i];

  while (str[i] && str[i] != '\t' && str[i] != ' ' && str[i] != '\n')
    i++;
  str[i] = '\n';
  str[i + 1] = 0;
  return res;
}

int main()
{
  int len;
  char s[MAXLEN];
  while ((len = readln(s, MAXLEN)) != 0)
    {
      if (len == 1)
	continue;
      printf("len=%d\n", len);
      printf("TRIMMED:%s", trimstr(s));
    }
}


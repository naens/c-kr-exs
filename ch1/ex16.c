#include <stdio.h>

#define MAXLINE 10

int readln(char s[], int lim)
{
  int c;
  int i = 0;
  while ((c = getchar()) != EOF)
    {
      if (i < lim)
	s[i] = c;
      i++;
      if (c == '\n')
	break;
    }
  s[i] = 0;
  return i;
}

int main()
{
  char s[MAXLINE];
  int len = readln(s, MAXLINE);
  if (len > 0)
    printf("string=\"%s\", len=%d\n", s, len);
}

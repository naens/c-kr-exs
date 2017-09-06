#include <stdio.h>

int slen(char *str)
{
  int i = 0;
  while (str[i])
    i++;
  return i;
}

void swapchr(char *c, char *d)
{
  char tmp = *c;
  *c = *d;
  *d = tmp;
}

void reverse(char *str)
{
  int len = slen(str);
  for (int i = 0; i < len / 2; i++)
    swapchr(&str[i], &str[len - i - 1]);
}

int main()
{
  int c;
  char str[0x1000];
  int i = 0;
  while ((c = getchar()) != EOF)
    {
      if (c == '\n')
	{
	  str[i] = 0;
	  i = 0;
	  reverse(str);
	  printf("%s\n", str);
	}
      else
	str[i++] = c;
    }
}

	  
	  

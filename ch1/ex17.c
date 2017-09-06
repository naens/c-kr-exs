#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 0x10000
#define LONG_LINE_LEN 12

int main()
{
  int c;
  char str[MAX_LINE_LEN];
  int len = 0;
  int is_long = 0;
  while ((c = getchar()) != EOF)
    {
      if (c == '\n')
	{
	  if (is_long)
	    {
	      str[len] = 0;
      	      printf("line is %s\n", str);
	    }
	  len = 0;
	  is_long = 0;
	}
      else
	{
	  str[len] = c;
	  len++;
	}
      is_long = len >= LONG_LINE_LEN;
    }
}

#include <stdio.h>

int main()
{
  int c;
  int in_word = 0;
  while ((c = getchar()) != EOF)
    {
      if (c == ' ' || c == '\t' || c == '\n')
	{
	  if (in_word)
	    {
	      in_word = 0;
	      putchar('\n');
	    }
	}
      else
	{
	  if (!in_word)
	    in_word = 1;
	  putchar(c);
	}
    }
}

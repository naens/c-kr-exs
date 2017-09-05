#include <stdio.h>

int main()
{
  int c;
  int prev_blank = 0;
  while ((c = getchar()) != EOF)
    {
      if (c != ' ')
	{
	  putchar(c);
	  prev_blank=0;
	}
      else if (!prev_blank)
	{
	  putchar(c);
	  prev_blank=1;
	}
    }
}


#include <stdio.h>

#define TAB_SZ 8
#define MAX_LINE 0x1000

void insert_char(char c, int n)
{
  for (int i = 0; i < n; i++)
    putchar(c);
}

int main()
{
  int chr;
  int col = 0;
  char line[0x1000];
  while ((chr = getchar()) != EOF)
    {
      if (chr == '\n')
	{
	  col = 0;
	  putchar(chr);
	}
      else if (chr == '\t')
	insert_char(' ', TAB_SZ - (col % TAB_SZ));
      else
	{
	  putchar(chr);
	  col++;
	}
      
    }
}

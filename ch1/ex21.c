#include <stdio.h>

#define TAB_SZ 8

void insert_char(char c, int n)
{
  for (int i = 0; i < n; i++)
    putchar(c);
}

void put_tabs(int col, int w)
{
  int n_tabs = (w + col % TAB_SZ) / TAB_SZ;
  int n_spaces = (n_tabs == 0) ? w : (w + col) % TAB_SZ;
  insert_char('\t', n_tabs);
  insert_char(' ', n_spaces);
}

int main()
{
  int c;
  int space_start;
  int space_len;
  int space_state = 0;
  int col = 0;
  while ((c = getchar()) != EOF)
    {
      if (c == ' ')
	{
	  if (space_state)
	    space_len++;
	  else
	    {
	      space_start = col;
	      space_len = 1;
	      space_state = 1;
	    }
	  col++;
	}
      else if (c == '\t')
	{
	  int tab_len = TAB_SZ - (col % TAB_SZ);
	  if (space_state)
	    space_len += tab_len;
	  else
	    {
	      space_start = col;
	      space_len = tab_len;
	      space_state = 1;
	    }
	  col += tab_len;
	}
      else if (c == '\n')
	{
	  if (space_state)
	    {
	      put_tabs(space_start, space_len);
	      space_state = 0;
	    }
	  col = 0;
	  putchar('\n');
	}
      else
	{
	  if (space_state)
	    {
	      put_tabs(space_start, space_len);
	      space_state = 0;
	    }
	  putchar(c);
	  col++;
	}
    }
}

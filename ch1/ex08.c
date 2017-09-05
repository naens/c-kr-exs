#include <stdio.h>

int main()
{
  int c;
  int blank_count = 0;
  int tab_count = 0;
  int nl_count = 0;
  while ((c = getchar()) != EOF)
    {
      switch (c)
	{
	case ' ':
	  blank_count++;
	  break;
	case '\t':
	  tab_count++;
	  break;
	case '\n':
	  nl_count++;
	  break;
	}
    }
  printf("blanks: %d\ntabs: %d;\nnew lines: %d\n", blank_count, tab_count, nl_count);
}

#include <stdio.h>

/* Fold long input into shorter lines before n-th column of input.
 * Fold after the last non-blank character.
 * End of line characters (displayed after margin):
 *  ->  ' ': automatic new line
 *  ->  '<': real new line
 *  ->  '+': line ending in new line or automatic new line, but
 *           too long to be displayed completely
 */

#define MAX_LINE_LENGTH 12
#define MARGIN_WIDTH 4
#define TAB_WIDTH 8
#define MAX_BUF_SIZE 0x10000

void insert_char(char c, int n)
{
  for (int i = 0; i < n; i++)
    putchar(c);
}

void print_margin(char c)
{
  insert_char(' ', MARGIN_WIDTH - 1);
  putchar(c);
}

void print_word(int space_len, char *buf, int length, int max_len)
{
  insert_char(' ', space_len);
  for (int i = 0; i < length && i < max_len; i++)
    putchar(buf[i]);
}

int main()
{
  int c;
  int i = 0;
  int cut_place = 0;
  int in_word = 0;
  int space_len = 0;
  char buf[MAX_BUF_SIZE];
  while ((c = getchar()) != EOF)
    {
      if (in_word)
	{
	  if (c == ' ' || c == '\t' || c == '\n')
	    {
	      if (i > MAX_LINE_LENGTH)
		{
		  if (cut_place)
		    printf("\n");
		  print_word(0, buf, i - cut_place, MAX_LINE_LENGTH);
		  i -= cut_place;
		}
	      else
		print_word(space_len, buf, i - cut_place, MAX_LINE_LENGTH - cut_place);
	      in_word = 0;
	    }
	  if (c == ' ')
	    {
	      space_len = 1;
	      i++;
	    }
	  else if (c == '\t')
	    {
	      int tab_len = TAB_WIDTH - (i % TAB_WIDTH);
	      space_len = tab_len;
	      i += tab_len;
	    }
	  else if (c == '\n')
	    {
	      insert_char(' ', MAX_LINE_LENGTH - i);
	      if (i - cut_place > MAX_LINE_LENGTH)
		print_margin('+');
	      else
		print_margin('<');
	      putchar('\n');
	      i = 0;
	      cut_place = 0;
	      space_len = 0;
	    }
	  else
	    {
	      buf[i - cut_place] = c;
	      i++;
	    }
	}
      else
	{
	  if (c == ' ')
	    {
	      space_len++;
	      i++;
	    }
	  else if (c == '\t')
	    {
	      int tab_len = TAB_WIDTH - (i % TAB_WIDTH);
	      space_len += tab_len;
	      i += tab_len;
	    }
	  else if (c == '\n')
	    {
	      print_word(0, buf, i - cut_place, MAX_LINE_LENGTH);
	      insert_char(' ', MAX_LINE_LENGTH - i);
	      if (i - cut_place > MAX_LINE_LENGTH)
		print_margin('+');
	      else
		print_margin('<');
	      putchar('\n');
	      i = 0;
	      cut_place = 0;
	      space_len = 0;
	    }
	  else /* begin new word => new cut_place */
	    {
	      cut_place = i;
	      in_word = 1;
	      buf[0] = c;
	      i++;
	    }
	}
    }
}

#include <stdio.h>
#include <math.h>

/* In order to be able to include this project in my repository I decided to
 * write my own version of the calculator and not take any code from the
 * book.
 *
 * Specifications
 *  + user enters input line by line
 *  + each line is an expression in poish notation
 *  + the stack and the stack pointer are global variables
 *  + the stack is represented as an array
 *  + the stack is accessible through push and pop functions
 *  + supported operations: +, -, *, /, %, sin, exp, pow
 *  + the numbers are in signed int or in double notation
 *  + write functions getch and ungetch, that work with a buffer (!!EOF!!)
 *  + write function ungets that works with strings
 */

struct elm {
  int typ;
  int val;
};

char *read_elm()
{
  static char buf[0x10000];
  static int next_eol=0;
  if (next_eol)
    {
      buf[0] = '\n';
      buf[1] = 0;
      next_eol = 0;
    }
  int chr = getchar();
  while (chr == '\t' || chr == ' ')
    chr = getchar();
  int i = 0;
  while (1)
    {
      if (chr == '\n')
        {
          /* if i = 0, return end-of-line */
          if (i == 0)
            {
              buf[i++] = chr;
              break;
            }
          /* if i > 0, return element, set next-end-of-line=true */
          else
            {
              next_eol = 1;
              break;
            }
        }
      else if (chr == '\t' || chr == ' ')
        {
          next_eol = 0;
          break;
        }
      else
        buf[i++] = chr;
      chr = getchar();
    }
  buf[i] = 0;
  return buf;
}

struct elm *parse_elm(char *c)
{
  static struct elm elm;
  return &elm;
}

void work_with(struct elm *elm)
{
}

int pop()
{
}

int main(int argc, char **argv)
{
  if (argc == 1)                /* stdin mode */
    {
      struct elm *curr;
      while ((curr = parse_elm(read_elm())))
        {
          printf("type=%d, value=%d\n", curr->typ, curr->val);

        }
    }
  else                          /* read arguments, print result */
    {
      for (int i = 1; i < argc; i++)
        work_with(parse_elm(argv[i]));
      printf("%d\n", pop());
    }
}

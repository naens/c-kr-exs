#include <stdio.h>

/* remove laa comments from a C program */

/* states:
 *     -> normal
 *     -> string: between "
 *     -> chars: between '
 *     -> escape: after \
 *     -> single-line-comments: between // and \n
 *     -> multiline comments
 */

/* automaton:
 *   normal:
 *     -> " : start string
 *     -> ' : start char
 *     -> / : start maybe-comment
 *   string:
 *     -> " : start normal
 *     -> \ : start string-escape
 *   char:
 *     -> \ : start char-escape
 *     -> else: start char-end
 *   maybe-comment:
 *     -> / : start single-line-comment
 *     -> * : start multiline-comment
 *     -> else: test normal
 *   string-escape:
 *     -> anything: string
 *   char-escape:
 *     -> anything: char-end
 *   char-end:
 *     -> ' : start normal
 *   single-line-comment:
 *     -> \n: normal
 *   multiline-comment:
 *     -> * : multiline-comment-maybe-end
 *   multiline-comment-maybe-end:
 *     -> /: multiline-comment-end
 *     -> else: multiline-comment
 *   multiline-comment-end:
 *     -> anything: test normal
 */

#define STATE_NORMAL 0
#define STATE_STRING 1
#define STATE_CHAR 2
#define STATE_MAYBE_COMMENT 3
#define STATE_STRING_ESCAPE 4
#define STATE_CHAR_ESCAPE 5
#define STATE_CHAR_END 6
#define STATE_SINGLE_LINE_COMMENT 7
#define STATE_MULTILINE_COMMENT 8
#define STATE_MULTILINE_COMMENT_MAYBE_END 9
#define STATE_MULTILINE_COMMENT_END 10

int main()
{
  int c;
  int state = STATE_NORMAL;
  int maybe_print = -1;
  while ((c = getchar()) != EOF)
    {
    e:
      switch (state)
        {
        case STATE_NORMAL:
          if (c == '"')
            state = STATE_STRING;
          else if (c == '\'')
            state = STATE_CHAR;
          else if (c == '/')
            state = STATE_MAYBE_COMMENT;
          break;
        case STATE_STRING:
          if (c == '"')
            state = STATE_NORMAL;
          else if (c == '\\')
            state = STATE_STRING_ESCAPE;
          break;
        case STATE_CHAR:
          if (c == '\\')
            state = STATE_CHAR_ESCAPE;
          else
            state = STATE_CHAR_END;
          break;
        case STATE_MAYBE_COMMENT:
          if (c == '/')
            state = STATE_SINGLE_LINE_COMMENT;
          else if (c == '*')
            state = STATE_MULTILINE_COMMENT;
          else 
            {state = STATE_NORMAL;goto e;} /* ANOMALY */
          break;
        case STATE_STRING_ESCAPE:
          state = STATE_STRING;
          break;
        case STATE_CHAR_ESCAPE:
          state = STATE_CHAR_END;
          break;
        case STATE_CHAR_END:
          if (c == '\'')
            state = STATE_NORMAL;
          break;
        case STATE_SINGLE_LINE_COMMENT:
          if (c == '\n')
            state = STATE_NORMAL;
          break;
        case STATE_MULTILINE_COMMENT:
          if (c == '*')
            state = STATE_MULTILINE_COMMENT_MAYBE_END;
          break;
        case STATE_MULTILINE_COMMENT_MAYBE_END:
          if (c == '/')
            state = STATE_MULTILINE_COMMENT_END;
          else
            state = STATE_MULTILINE_COMMENT;
          break;
        case STATE_MULTILINE_COMMENT_END:
          {state = STATE_NORMAL; goto e;} /* ANOMALY */
        }
      if (state == STATE_MAYBE_COMMENT)
        maybe_print = c;
      else if (state == STATE_SINGLE_LINE_COMMENT ||
          state == STATE_MULTILINE_COMMENT)
        maybe_print = -1;
      else if (state != STATE_MULTILINE_COMMENT_MAYBE_END &&
               state != STATE_MULTILINE_COMMENT_END)
        {
          if (maybe_print != -1)
            {
              putchar(maybe_print);
              maybe_print = -1;
            }
      
          putchar(c);
        }
    }
}


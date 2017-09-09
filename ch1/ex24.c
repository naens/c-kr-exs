#include <stdio.h>
#include <stdlib.h>

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
 *     -> " : start string
 *     -> ' : start char
 *     -> else: start normal
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
 *     -> " : start string
 *     -> ' : start char
 *     -> / : start maybe-comment
 *     -> else: start normal
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

#define PAREN 0
#define BRACKET 1
#define BRACE 2
#define ESTACK 0x10

struct stack {
  int val;
  struct stack *next;
};

int stack_pop(struct stack **pstack)
{
  if (*pstack)
    {
      int result = (*pstack)->val;
      struct stack *tmp = *pstack;
      *pstack = (*pstack)->next;
      free(tmp);
      return result;
    }
  return -1;
}

void stack_push(struct stack **pstack, int val)
{
  struct stack *new = malloc(sizeof (struct stack));
  new->val = val;
  new->next = *pstack;
  *pstack = new;
}

int work_with2(char c, struct stack **pstack)
{
  switch(c)
    {
    case '(':
      stack_push(pstack, PAREN);
      break;
    case '[':
      stack_push(pstack, BRACKET);
      break;
    case '{':
      stack_push(pstack, BRACE);
      break;
    case ')':
      if (stack_pop(pstack) != PAREN)
        return PAREN | ESTACK;
      break;
    case ']':
      if (stack_pop(pstack) != BRACKET)
        return BRACKET | ESTACK;
      break;
    case '}':
      if (stack_pop(pstack) != BRACE)
        return BRACKET | ESTACK;
      break;
    }
  return 0;
}

void work_with(char c, struct stack **pstack)
{
  int r = work_with2(c, pstack);
  if (r & ESTACK)
    {
      if (r & PAREN)
        fprintf(stderr, "closing parenthese error\n");
      if (r & BRACKET)
        fprintf(stderr, "closing bracket error\n");
      if (r & BRACE)
        fprintf(stderr, "closing brace error\n");
    }
}

void check_stack(struct stack *stack)
{
  if (stack)
    switch (stack->val)
      {
      case PAREN:
        fprintf(stderr, "open parenthese error\n");
        break;
      case BRACKET:
        fprintf(stderr, "open bracket error\n");
        break;
      case BRACE:
        fprintf(stderr, "open brace error\n");
        break;
      }
}

int main()
{
  int c;
  int state = STATE_NORMAL;
  int maybe_print = -1;
  struct stack *stack = 0;
  while ((c = getchar()) != EOF)
    {
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
          else if (c == '"')
            state = STATE_STRING;
          else if (c == '\'')
            state = STATE_CHAR;
          else
            state = STATE_NORMAL;
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
          if (c == '"')
            state = STATE_STRING;
          else if (c == '\'')
            state = STATE_CHAR;
          else if (c == '/')
            state = STATE_MAYBE_COMMENT;
          else
            state = STATE_NORMAL;
          break;
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
              work_with(maybe_print, &stack);
              maybe_print = -1;
            }
      
          work_with(c, &stack);
        }
    }
  check_stack(stack);
}


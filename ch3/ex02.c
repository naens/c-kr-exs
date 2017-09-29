#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void escape(char *s, char *t)
{
  unsigned char c;
  while (c = *t)
    {
      switch (c)
        {
        case '\t':
          s += sprintf(s, "\\t");
          break;
        case '\n':
          s += sprintf(s, "\\n");
          break;
        default:
          if (c >= 0x20 && c <= 0x7e)
            *(s++) = c;
          else
            s += sprintf(s, "\\x%02X", c);
        }
      t++;
    }
  *t = 0;
}

int dg2ch(char c1, char c2)
{
  printf("dg2ch: %02x %02x\n", c1, c2);
  int tmp;
  if (c1 >= '0' && c1 <= '9')
    tmp = (c1 - '0') * 0x10;
  else if (c1 >= 'a' && c1 <= 'z')
    tmp = (c1 - 'a' + 10) * 0x10;
  else if (c1 >= 'A' && c1 <= 'Z')
    tmp = (c1 - 'A' + 10) * 0x10;
  else
    tmp = 0;

  if (c2 >= '0' && c2 <= '9')
    tmp += c2 - '0';
  else if (c2 >= 'a' && c2 <= 'z')
    tmp += c2 - 'a' + 10;
  else if (c2 >= 'A' && c2 <= 'Z')
    tmp += c2 - 'A' + 10;

  printf("dg2ch: tmp=%02X\n", tmp);


  return tmp;
}

void unescape(char *s, char *t)
{
  int esc = 0;
  unsigned char c;
  while (c = *t)
    {
      if (esc == 1)
        {
          if (c == 'x')
            esc = 2;
          else
            switch (c)
              {
              case 't':
                *(s++) = '\t';
                esc = 0;
                break;
              case 'n':
                *(s++) = '\n';
                esc = 0;
                break;
              default:
                *(s++) = c;
                esc = 0;
                break;
              }
        }
      else if (esc == 2) /* first digit */
        {
          if (c >= '0' && c <='9' || c >= 'a' && c <= 'z' || c >='A' && c <= 'Z')
            esc++;
          else                  /* thought it was a number but nope */
            {
              s += sprintf(s, "\\x");
              esc = 0;
              t--;              /* weird thing here: "unread" the current character */
            }
        }
      else if (esc == 3) /* second digit */
        {
          if (c >= '0' && c <='9' || c >= 'a' && c <= 'z' || c >='A' && c <= 'Z')
            *(s++) = dg2ch(*(t - 1), c);
          else
            s += sprintf(s, "%c%c", dg2ch(0, *(t - 1)), c);
          esc = 0;
        }
      else if (c == '\\')
        esc = 1;
      else
        *(s++) = c;
      t++;
    }
  *t = 0;
}

int main(int argc, char **argv)
{
  if (argc != 3)
    {
      fprintf(stderr, "usage: %s <escape|unescape> <string>\n", argv[0]);
      exit(1);
    }

  char buf[0x10000];
  if (strcmp(argv[1], "escape") == 0)
    escape(buf, argv[2]);
  else
    unescape(buf, argv[2]);

  printf("%s\n", buf);
}

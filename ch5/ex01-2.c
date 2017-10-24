#include <stdio.h>

#define IS_DIGIT(x) (((x)>='0')&&((x)<='9'))
#define DIGIT(x) ((x)-'0')

/* returns number of char read if not EOF, EOF if EOF */
int getint(int *n)
{
  *n = 0;
  int c;
  c = getc(stdin);
  while (c == '\n' || c == ' ' || c == '\t')
    c = getc(stdin);

  int result = 1;
  if (c == EOF)
    {
      ungetc(c, stdin);
      return EOF;
    }
  char sign_char = 0;
  if (c == '+' || c == '-')
    {
      sign_char = c;
      c = getc(stdin);
      result++;
    }
  int sign;
  sign = sign_char == '-' ? -1 : 1;
  if (!IS_DIGIT(c))
    {
      if (sign_char)
	ungetc(sign_char, stdin);
      return 0;
    }
  *n = DIGIT(c);
  while (c = getc(stdin), IS_DIGIT(c))
    {
      *n = *n * 10 + DIGIT(c);
      result++;
    }

  *n *= sign;

  ungetc(c, stdin);
  return result;
}

int getfloat(double *n)
{
  *n = 0;
  int n1;
  int res;
  res = getint(&n1);
  if (res == EOF)
    return EOF;

  *n = n1;
  int dot = getc(stdin);
  if (dot != '.')
    {
      ungetc((char)dot, stdin);
      return res;
    }

  int tmp1_chr = getc(stdin);
  if (tmp1_chr == EOF)
    {
      ungetc('.', stdin);
      return res;
    }

  ungetc((char)tmp1_chr, stdin);
  if (!IS_DIGIT(tmp1_chr))
    return res;

  int n2;
  int res2;
  res2 = getint(&n2);
  if (res2 == EOF)
    return EOF;
  res += 1 + res2;

  double n2d = n2;
  for (int i = 0; i < res2; i++)
    n2d /= 10;

  int echr = getc(stdin);
  int sign = *n < 0 ? -1 : 1;
  *n += sign * n2d; 
  if (echr != 'e')
    {
      ungetc((char)echr, stdin);
      return res;
    }

  int tmp2 = getc(stdin);
  if (tmp2 == EOF)
    {
      ungetc('e', stdin);
      return res;
    }

  ungetc((char)tmp2, stdin);
  if (!IS_DIGIT(tmp2) && tmp2 != '-' && tmp2 != '+' )
    return res;

  int n3;
  int res3;
  res3 = getint(&n3);
  if (res3 == EOF)
    return res;

  res += 1+ res3;

  double k = n3 > 0 ? 10 : 0.1;
  int n3sign = n3 < 0 ? -1 : 1;
  for (int i = 0; i < n3 * n3sign; i++)
    *n *= k;

  return res;
}

int main(int argc, char **argv)
{
  double n;
  int res = getfloat(&n);
  while (res > 0 && res != EOF)
    {
      printf("n=%f res=%d\n", n, res);
      res = getfloat(&n);
    }
}

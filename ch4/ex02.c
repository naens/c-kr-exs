#include <stdio.h>

char *read_sign(char *s, int *sign)
{
  if (*s == '-')
    {
      *sign = -1;
      return s + 1;
    }
  else
    {
      *sign = 1;
      return s;
    }
}

char *read_number(char *s, long long int *n)
{
  *n = 0;
  while (*s >= '0' && *s <= '9')
    {
      *n *= 10;
      *n += *s - '0';
      s++;
    }
  return s;
}

double expt(double x, int n)
{
  if (n > 0)
    for (int i = 0; i < n; i++)
      x *= 10;
  else
    for (int i = 0; i > n; i--)
      x /= 10;
  return x;
}

double atof(char *s)
{
  int sign;
  long long int n;
  s = read_sign(s, &sign);
  s = read_number(s, &n);

  if (*s == '.')
    s++;

  long long int n2;
  char *s2;
  int n2len;
  s2 = read_number(s, &n2);
  n2len = s2 - s;
  s = s2;

  if (*s == 'e' || *s == 'E')
    s++;

  int e_sign;
  long long int e;
  s = read_sign(s, &e_sign);
  s = read_number(s, &e);

  printf("[%d %d][%d][%d %d]\n", sign, n, n2, e_sign, e);

  return expt(sign * (n + expt(n2, -1 * n2len)), e_sign * e);
}

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "wrong arguments\n");
      return -132409;
    }

  printf("%f\n", atof(argv[1]));
}

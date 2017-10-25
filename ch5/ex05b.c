#include <stdio.h>
#include <stdlib.h>

int ncat(char *s, char *t, int n)
{
    char *t0 = t;
    while (*s)
      s++;
    while (n > 0 && (*s = *t))
      n--, s++, t++;
    *s = 0;
    return t - t0;
}

int main (int argc, char **argv)
{
    char *s = argc > 2 ? argv[1] : argv[0];
    char *t = argc > 3 ? argv[2] : "hello";
    int n = argc < 2 ? 5 : atol(argv[3]);
    int res = ncat(s, t, n);
    printf("%s res=%d\n", s, res);
}

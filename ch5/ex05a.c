#include <stdio.h>
#include <stdlib.h>

int ncopy(char *s, char *t, int n)
{
    printf("ss=%s n=%d\n", s, n);
    char *t0 = t;
    while (n > 0 && (*t = *s))
        s++, t++, n--;
    *t = 0;
    return t - t0;
}

int main (int argc, char **argv)
{
    char *s = argc > 1 ? argv[1] : argv[0];
    int n = argc < 2 ? 5 : atol(argv[2]);
    char buf[0x100];
    int res = ncopy(s, buf, n);
    printf("%s res=%d\n", buf, res);
}

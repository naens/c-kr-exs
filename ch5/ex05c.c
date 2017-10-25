#include <stdio.h>
#include <stdlib.h>

int ncmp(char *s, char *t, int n)
{
    while (*s == *t && *s && *t && n > 0)
      s++, t++, n--;
    return *s - *t;
}

int main(int argc, char **argv)
{
    char *s = argc >= 2 ? argv[1] : argv[0];
    char *t = argc >= 3 ? argv[2] : argv[0];
    int n = argc >= 4 ? atol(argv[3]) : 5;
    printf("%d\n", ncmp(s, t, n));
}

#include <stdio.h>

void reverse(char *s)
{
    char *s0 = s;
    while (*s)
      s++;
    char c;
    s--;
    while (s0 < s)
    {
        c = *s0;
        *s0 = *s;
        *s = c;
        s0++, s--;
    }
}

int main(int argc, char **argv)
{
    char *s = argc >= 2 ? argv[1] : argv[0];
    reverse(s);
    printf("%s\n", s);
}

#include <stdio.h>
#include <stdlib.h>

void scan(char fmt, void *a)
{
    int c;
    int i = 0;
    char buf[0x100];
    while ((c = getchar()) != EOF)
    {
        if (c == ' ' || c == '\t' || c == '\n')
            continue;
        buf[i++] = c;
    }
    buf[i] = 0;
    if (i == 0)
        *a = NULL;
    else if (fmt == 'c')
        *a = buf[0];
    else
        *a = atoi(buf);
}

int main(int argc, char **argv)
{
}

#include <stdio.h>
#include <string.h>

int error(char *msg, int n)
{
    fprintf(stderr, "%s\n", msg);
    return n;
}

int tolower(int c)
{
    return (c >= 'A' && c <= 'Z') ? (c + 0x20) : c;
}

int toupper(int c)
{
    return (c >= 'a' && c <= 'z') ? (c - 0x20) : c;
}

int main(int argc, char **argv)
{
    int low = strcmp(argv[0], "low") == 0;
    int c;
    while ((c = getchar()) != EOF)
        putchar(low ? tolower(c) : toupper(c));
    return 0;
}

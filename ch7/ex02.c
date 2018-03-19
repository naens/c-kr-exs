#include <stdio.h>

void printbuf(char *buf, int len)
{
    for (int i = 0; i < len; i++)
        putchar((buf[i] >= ' ' && buf[i] <= 0xfe) ? buf[i] : '.');
}

int main(int argc, char **argv)
{
    int c;
    int len = 0;
    char buf[8];
    while ((c = getchar()) != EOF)
    {
        buf[len] = c;
        printf("%02x ", c);
        len++;
        if (len == 8)
        {
            printf("    ");
            len = 0;
            printbuf(buf, 8);
            putchar('\n');
        }
    }
    if (len > 0)
        printbuf(buf, len);
}

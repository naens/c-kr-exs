#include <stdio.h>
#include <stdarg.h>

/* printf-like function, implementing %d, %f, %s, %c, %o, %x */
void pf(char *str, ...)
{
    va_list ap;
    va_start(ap, str);
    char *p = str;
    while (*p != '\0')
    {
        if (*p != '%')
            putchar(*p);
        else
        {
            p++;
            if (p == 0)
                break;
            switch (*p) {
            case 'd':
                printf("%d", va_arg(ap, int));
                break;
            case 'x':
                printf("%o", va_arg(ap, int));
                break;
            case 'o':
                printf("%o", va_arg(ap, int));
                break;
            case 'c':
                printf("%d", va_arg(ap, int));
                break;
            case 'f':
                printf("%f", va_arg(ap, double));
                break;
            case 's':
                printf("%s", va_arg(ap, char*));
                break;
            default:
                break;
            }
        }
        p++;
    }
    va_end(ap);
}

int main(int argc, char **argv)
{
    pf("abcde%d\n", 45);
    pf("abcde%s-%x\n", "LKJ", 45);
    return 0;
}

#include <stdio.h>

/* return number of characters skipped */
int skip_spaces()
{
    int c;
    int res = 0;
    while((c = getchar()) != EOF)
    {
        if (c != ' ' && c != '\t')
            break;
        res++;
    }
    if (c != EOF)
        ungetc(c, stdin);
    return res;
}

int skip_comments()
{
    int c = getchar();
    if (c == EOF)
        return 0;
    if (c != '/')
    {
        ungetc(c, stdin);
        return 0;
    }
    c = getchar();
    if (c == EOF)
    {
        ungetc('/', stdin);
        return 0;
    }
    if (c == '/')
    {
        int res = 2;
        do
        {
            c = getchar();
            res++;
        } while (c != EOF && c != '\n');
        return res;
    }
    else if (c == '*')
    {
        int res = 2;
        int c2 = getchar();
        while (c2 != EOF && !(c == '*' && c2 == '/'))
        {
            c = c2;
            c2 = getchar();
            res++;
        }
        return res;
    }
    else
    {
        ungetc('/', stdin);
        return 0;
    }
}

int is_word_char(char c)
{
    return c >= 'a' && c <= 'z'
        || c >= 'A' && c <= 'Z'
        || c >= '0' && c <= '9'
        || c == '_';
}

int get_word(char *word, int lim)
{
    while (skip_spaces() || skip_comments())
        ;
    int c = getchar();
    if (!is_word_char(c) || c == EOF)
    {
        *word = 0;
        return c;
    }

    int first = c;
    *word++ = c;
    while (lim > 1 && is_word_char(c = getchar()))
        *(word++) = c, lim--;
    *word = 0;
    if (c != EOF)
        ungetc(c, stdin);
    return first;
}
    
int main()
{
    char buf[0x1000];
    get_word(buf, 5);
    printf("%s\n", buf);
}

    
    
    
    
    
    
    
    
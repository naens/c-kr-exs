#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

void read_arg(char *arg, int *m, int *n)
{
    switch (*arg)
    {
    case '-':
        *m = atoi(arg + 1);
        break;
    case '+':
        *n = atoi(arg + 1);
        break;
    }
}

void insert_char(char c, int n)
{
    for (int i = 0; i < n; i++)
      putchar(c);
}

void insert_space_tabs(int from, int to, int m, int n)
{
    if (to < m)
      insert_char(' ', from - to);
    else if (from < m)
    {
        insert_char('\t', 1);
        int k = to - m;
        insert_char('\t', k / n);
        insert_char(' ', k % n);
    }
    else
    {
        int a = from - m;
        int b = to - m;
        if (a / n < b / n)
          a = a - (a % n);
        int k = b - a;
        insert_char('\t', k / n);
        insert_char(' ', k % n);
    }
}

void entab(int m, int n)
{
    int c;
    int i = 0;
    int space_start;
    int space_state = 0;
    while ((c = getchar()) != EOF)
    {
        if (c == ' ')
        {
            if (!space_state)
            {
                space_start = i;
                space_state = 1;
            }
            i++;
        }
        else if (c == '\t')
        {
            if (!space_state)
            {
                space_start = i;
                space_state = 1;
            }
            i = i < m ? m : m + ((i - m) - ((i - m) % n)) + n;
        }
        else if (c == '\n')
        {
            space_state = 0;
            i = 0;
            putchar(c);
        }
        else if (space_state)
        {
            space_state = 0;
            insert_space_tabs(space_start, i, m, n);
            putchar(c);
            i++;
        }
        else
        {
            putchar(c);
            i++;
        }
    }
}

void detab(int m, int n)
{
    int c;
    int i = 0;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            i = 0;
            putchar(c);
        }
        else if (c == '\t')
        {
            int spaces = i < m ? m - i : (n - (i - m) % n);
            i += spaces;
            for (int j = 0; j < spaces; j++)
              putchar(' ');
        }
        else
        {
            i++;
            putchar(c);
        }
    }
}

int main(int argc, char **argv)
{
    char *prog_name = basename(argv[0]);
    int m = 0;
    int n = 8;
    for (int i = 1; i < argc; i++)
      read_arg(argv[i], &m, &n);
    printf("m = %d, n = %d\n", m, n);
    if (strcmp(prog_name, "entab") == 0)
    {
        printf("ENTAB\n");
        entab(m, n);
    }
    else if (strcmp(prog_name, "detab") == 0)
    {
        printf("DETAB\n");
        detab(m, n);
    }
    else
    {
        fprintf(stderr, "Unrekognized program name: '%s'\n", prog_name);
    }
}

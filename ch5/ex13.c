#include <stdio.h>
#include <stdlib.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];

int readln(int n)
{
    int c;
    int i = 0;
    lineptr[n] = malloc(MAXLINES * (sizeof (char *)));
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
            break;
        lineptr[n][i++] = c;
    }
    return c;
}

int main(int argc, char **argv)
{
    int n = 10;
    if (argc > 1)
      n = -atoi(argv[1]);

    if (n < 0 || n > MAXLINES)
    {
        fprintf(stderr, "bad number of lines\n");
        exit -1;
    }

    int line = 0;
    while (readln(line) != EOF)
    {
        line = (line + 1) % MAXLINES;
        if (lineptr[line])
          free(lineptr[line]);
    }
    
    line = (line + MAXLINES - n) % MAXLINES;
    for (int i = 0; i < n; i++)
      printf("%s\n", lineptr[(line + i) % MAXLINES]);

    for (int i = 0; i < n; i++)
      if (lineptr[i])
          free(lineptr[i]);
}

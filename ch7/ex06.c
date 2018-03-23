#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINESZ 0x100

void error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        error("args: <file> <file>");

    char *fn1 = argv[1];
    char *fn2 = argv[2];
    FILE *f1 = fopen(fn1, "r");
    FILE *f2 = fopen(fn2, "r");
    if (f1 == NULL || f2 == NULL)
        error("file error");

    char buf1[MAXLINESZ];
    char buf2[MAXLINESZ];
    while (fgets(buf1, MAXLINESZ, f1) != NULL
     && fgets(buf2, MAXLINESZ, f2) != NULL)
    {
        if (strcmp(buf1, buf2) != 0)
        {
            printf("%s", buf1);
            printf("%s", buf2);
            break;
        }
    }
    return 0;        
}

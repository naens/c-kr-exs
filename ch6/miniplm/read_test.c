#include <stdio.h>
#include "miniplm.h"

int main(int argc, char **argv)
{
    FILE *f;
    if (argc == 2)
    {
        f = fopen(argv[1], "r");
        if (f == NULL)
        {
            fprintf(stderr, "could not open %s\n", argv[1]);
            return 1;
        }
    }
    else
        f = stdin;

    int res;
    struct element element;
    while ((res = read_token(f, &element)) != EOF)
    {
        char buf[0x100];
        sprint_elem_json(buf, &element, 1);
        printf("%s\n", buf);
    }

    printf("\n");
    return 0;
}

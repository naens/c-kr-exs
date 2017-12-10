#include <stdio.h>
#include "reader.h"

int main(int argc, char **argv)
{
    FILE* f;
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

    /* TODO: read tokens from f and display them */
            
}

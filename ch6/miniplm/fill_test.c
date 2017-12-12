#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "miniplm.h"
#include "parse.h"
#include "vardata.h"
#include "read_ast.h"

void print_var(struct element *element)
{
    /* TODO */
}

int main(int argc, char **argv)
{
    FILE *file;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return -1;
    }
    file = fopen(argv[1], "r");
    if  (file == NULL)
    {
        fprintf(stderr, "error opening file '%s'\n", argv[1]);
        return 1;
    }
    struct element element;
    int res = start(file, &element);
    if (!res)
    {
        fprintf(stderr, "\"bad input\"\n");
        return 7;
    }

    /* fill block id data and var map */
    struct var_map_node **var_map = init_var_map();
    fill_ast(&element, var_map);

    /* print blocks, var/proc decl, var/proc in code */
    print_var(&element);

    /* free */
    var_map_free(var_map);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "miniplm.h"
#include "parse.h"
#include "vardata.h"
#include "read_ast.h"

/* print proc/vars declarations and uses */
static int proc_decl = 0;
void print_vars(struct element *element, struct var_map_node **var_map)
{
    if (element->elem_term == NONTERMINAL)
    {
        switch (element->type)
        {
        case PROCEDURE:
            printf("proc: block_id=%d  ", element->block_id);
            proc_decl = 1;
            break;
        case DECL_ELEMENT:
            printf("variable:  ");
            break;
        case BLOCK_END:
            proc_decl = 0;
        default:
            break;
        }
        struct elem_list *elem_list = element->val.elem_list;
        while (elem_list != NULL)
        {
            print_vars(elem_list->element, var_map);
            elem_list = elem_list->next;
        }
    }
    else
    {
        if (element->type == IDENT)
        {
            if (proc_decl)
                printf("proc-id/var: %s:%d\n",
                    element->val.str, element->block_id);
            else
                printf("var: %s:%d\n", element->val.str, element->block_id);
        }
    }
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
    print_vars(&element, var_map);

    /* free */
    var_map_free(var_map);

    return 0;
}

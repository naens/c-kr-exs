#include <stdio.h>

#include "miniplm.h"
#include "parse.h"
#include "vardata.h"
#include "read_ast.h"

struct element *find(struct element *element, enum type type)
{
    struct elem_list *list = element->val.elem_list;
    while (list != NULL && list->element->type != type)
        list = list->next;
    if (list != NULL)
        return list->element;
    else
        return NULL;
}

void exec_block_end(struct var_map_node **var_map,
                    struct element *block_end, struct value *value)
{
    /* TODO: execute each unit in the block */

    /* remove current block variables from map */
    var_map_del_block(var_map, block_end->block_id);
}

void exec_proc(struct var_map_node **var_map, struct element *proc, 
               struct value **args, struct value *value)
{
    /* find arguments, assign as variables for current block */
    struct element *params = find(proc->val.elem_list->element, PARAMS);
    if (args != NULL && params != NULL)
    {
        struct elem_list *list = params->val.elem_list;
        while (list != NULL)
        {
            struct element *element = list->element;
            int i = 0;
            if (element->type == IDENT)
            {
                char *arg_name = element->val.str;
                int block_id = proc->block_id;
                struct value *val = args[i];
                var_map_add(var_map, arg_name, block_id, val);
                i++;
            }
            list = list->next;
        }        
    }

    /* execute the body */
    exec_block_end(var_map, find(proc, BLOCK_END), value);
}

void execute(struct element *start, struct var_map_node **var_map)
{
    struct element *do_block = find(start, DO_BLOCK);
    struct element *block_end =
                     find(do_block->val.elem_list->element, BLOCK_END);
    exec_block_end(var_map, block_end, NULL);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "no file argument\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "could not open file %s\n", argv[1]);
        return 1;
    }
        
    struct element start_element;
    if (!start(file, &start_element))
    {
        fprintf(stderr, "Error in file\n");
        return 1;
    }

    struct var_map_node **var_map = init_var_map();
    fill_ast(&start_element, var_map);

    execute(&start_element, var_map);

    var_map_free(var_map);

    return 0;
}

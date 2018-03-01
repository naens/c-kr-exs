#include <stdio.h>

#include "miniplm.h"
#include "parse.h"
#include "vardata.h"
#include "read_ast.h"

/* forward declarations */
struct value *exec_block_end(struct var_map_node **var_map,
                    struct element *block_end);


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

struct value *exec_cond(struct var_map_node **var_map,
                    struct element *element)
{
    return NULL;
}

struct value *exec_do_block(struct var_map_node **var_map,
                    struct element *element)
{
    struct element *block_end = find(do_block, BLOCK_END);
    return exec_block_end(var_map, block_end);
}

struct value *exec_do_while(struct var_map_node **var_map,
                    struct element *element)
{
    return NULL;
}

struct value *exec_do_iter(struct var_map_node **var_map,
                    struct element *element)
{
    return NULL;
}

struct value *exec_statement(struct var_map_node **var_map,
                    struct element *element)
{
    return NULL;
}

/* unit = cond | do_block | do_while | do_iter | statement */
struct value *exec_unit(struct var_map_node **var_map,
                    struct element *element)
{
    struct elem_list *list = element->val.elem_list;
    struct element *sub_element = list->element;
    switch (element->type) {
    case COND:
        return exec_cond(var_map, sub_element);
    case DO_BLOCK:
        return exec_do_block(var_map, sub_element);
    case DO_WHILE:
        return exec_do_while(var_map, sub_element);
    case DO_ITER:
        return exec_do_iter(var_map, sub_element);
    case STATEMENT:
        return exec_statement(var_map, sub_element);
    default:
        return NULL;
    }
}

struct value *exec_block_end(struct var_map_node **var_map,
                    struct element *block_end)
{
    printf("begin exec_block_end: %d\n", block_end->block_id);
    struct elem_list *list = block_end->val.elem_list;

    /* skip declarations */
    while (list != NULL && list->element->type == DECLARATION)
        list = list->next;

    while (list != NULL)
    {
        struct value *result;
        if (list->element->type == UNIT) {
            result = exec_unit(var_map, list->element);
            if (result != NULL)
                return result;
        }
        list = list->next;
    }

    /* remove current block variables from map */
    var_map_del_block(var_map, block_end->block_id);
    printf("end exec_block_end: %d\n", block_end->block_id);
    return NULL;
}

struct value *exec_proc(struct var_map_node **var_map, struct element *proc, 
               struct value **args)
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
    return exec_block_end(var_map, find(proc, BLOCK_END));
}

void execute(struct element *start, struct var_map_node **var_map)
{
    struct element *do_block = find(start->val.elem_list->element, DO_BLOCK);
    exec_do_block(var_map, do_block);
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

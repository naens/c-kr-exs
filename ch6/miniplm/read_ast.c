#include <stdio.h>
#include <stdlib.h>

#include "read_ast.h"

/*
 * procedure = proc_statement block_end
 * proc_statement= ident ":" "PROCEDURE" [ params ] [ type ]";"
 * params = "(" ident { "," ident } ")"
 */

char *get_proc_name(struct element *procedure)
{
    if (procedure->type != PROCEDURE)
        return NULL;
    struct element *proc_statement = procedure->val.elem_list->element;
    if (proc_statement->type != PROC_STATEMENT)
        return NULL;
    struct element *ident = proc_statement->val.elem_list->element;
    if (ident->type != IDENT)	
        return NULL;
    return ident->val.str;
}


/*
 * decl_element = decl_fact | decl_unfact
 * decl_unfact = ident type [ initial ]
 * decl_fact = "(" ident { "," ident } ")" type [ initial ]
 */

char **get_decl_names(struct element *decl_element)
{
    char **result;
    if (decl_element->type != DECL_ELEMENT)
        return NULL;
    struct element *decls = decl_element->val.elem_list->element;
    if (decls->type == DECL_UNFACT)
    {
        result = malloc(2 * sizeof(char*));
        result[0] = decls->val.elem_list->element->val.str;
        result[1] = NULL;
    }
    else if (decls->type == DECL_FACT)
    {
        int n = 0;
        struct elem_list *list = decls->val.elem_list->next;
        for (;;)
        {
            if (list->element->type == IDENT)
                n++;
            if (list->next->element->type == COMMA)
                list = list->next->next;
            else
                break;
        }
        if (n == 0)
            return NULL;
        list = decls->val.elem_list->next;
        result = malloc((n + 1) * sizeof(char*));
        int i;
        for (i = 0; i < n; i++, list = list->next->next)
            result[i] = list->element->val.str;
        result[i] = NULL;
    }
    else
        return NULL;
    return result;
}

static int proc_decl = 0;
static int proc_name = 0;
void fill_element(struct element *element,
                  struct name_table_node **name_table,
                  struct var_map_node **var_map,
                  int block_id)
{
    if (element->elem_term == NONTERMINAL)
    {
        switch (element->type)
        {
        case PROCEDURE:       ;    /* procedure declaration */
            char *name = get_proc_name(element);
            proc_decl = 1;
            proc_name = 1;
            name_table_add(name_table, name, block_id);
            block_id++;
          printf("procedure: \"%s\":%d\n", name, block_id);
            element->block_id = block_id;
            var_map_add(var_map, name, block_id, PROCEDURE, element);
            break;
        case DECL_ELEMENT:  ;     /* variable declaration */
            char **names = get_decl_names(element);
            char **tmp = names;
            while (*names != NULL)
            {
//              printf("decl_element: \"%s\":%d\n", *names, block_id);
                name_table_add(name_table, *names, block_id);
                names++;
            }
            free(tmp);
            break;
        case BLOCK_END:       ;    /* blocks and procedure blocks */
            if (!proc_decl)
                block_id++;
            else
                proc_decl = 0;
//          printf("block %d\n", block_id);
            break;
        default:
            break;
        }
        struct elem_list *plist = element->val.elem_list;
        while (plist != NULL)
        {
            fill_element(plist->element, name_table, var_map, block_id);
            plist = plist->next;
        }
        if (element->type == BLOCK_END)
        {
//          printf("del_block:%d\n", block_id);
            name_table_del_block(name_table, block_id);
        }
    }
    else if (element->type == IDENT)
    {
        /* identifier: procedure or variable */
        if (proc_decl)
        {
            if (proc_name)
            {
                proc_name = 0;
                element->block_id =
                    name_table_get_block(name_table, element->val.str);
            }
            else
            {
                name_table_add(name_table, element->val.str, block_id);
                element->block_id = block_id;
//              printf("proc identifier: \"%s\":%d\n",
//                  element->val.str, block_id);
            }
        }
        else
        {
            int var_block = name_table_get_block(name_table, element->val.str);
//          printf("identifier: \"%s\":%d\n", element->val.str, var_block);
            element->block_id = var_block;
        }
    }
}

void fill_ast(struct element *element, struct var_map_node **var_map)
{
    struct name_table_node **name_table = init_name_table();
    fill_element(element, name_table, var_map, 0);
    name_table_free(name_table);
}

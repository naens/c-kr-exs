#include <string.h>
#include <stdlib.h>

#include "vardata.h"

#define NAME_TABLE_SZ 16
#define VAR_TABLE_SZ 16

/* name table functions */

int name_table_hash(char *var_name)
{
    int result = 0;
    while (*var_name)
    {
        result += result * 51 + *var_name;
        var_name++;
    }
    return result;
}

struct name_table_node **init_name_table()
{
    return malloc(NAME_TABLE_SZ * sizeof(struct name_table_node*));
}

void name_table_add(struct name_table_node **name_table, char *var, int bl)
{
    int i = name_table_hash(var) % NAME_TABLE_SZ;
    struct name_table_node *node = name_table[i];
    while (node != NULL && strcmp(var, node->var_name) != 0)
        node = node->next;
    if (node == NULL)
    {
        node = malloc(sizeof(struct name_table_node));
        node->next = name_table[i];
        name_table[i] = node;
        node->block_id_stack = NULL;
    }
    struct block_id_stack *tmp = malloc(sizeof(struct block_id_stack));
    tmp->block_id = bl;
    tmp->next = node->block_id_stack;
    node->block_id_stack = tmp;
}

int name_table_get_block(struct name_table_node **name_table, char *var)
{
    int i = name_table_hash(var) % NAME_TABLE_SZ;
    struct name_table_node *node = name_table[i];
    while (node != NULL && strcmp(var, node->var_name) != 0)
        node = node->next;
    if (node == NULL)
        return -1;
    return node->block_id_stack->block_id;
}

void name_table_del_block(struct name_table_node **name_table, int block_id)
{
    for (int i = 0; i < NAME_TABLE_SZ; i++)
    {
        struct name_table_node **pnode = &name_table[i];
        while (*pnode != NULL)
        {
            if ((*pnode)->block_id_stack->block_id = block_id)
            {
                struct block_id_stack *tmp = (*pnode)->block_id_stack;
                (*pnode)->block_id_stack = tmp->next;
                if (tmp->next == NULL)
                {
                    struct name_table_node *tmp2 = *pnode;
                    *pnode = tmp2->next;
                    free(tmp2);
                }
                free(tmp);
            }
            pnode = &(*pnode)->next;
        }
    }    
}

void name_table_free(struct name_table_node **name_table)
{
    for (int i = 0; i < NAME_TABLE_SZ; i++)
    {
        struct name_table_node *node = name_table[i];
        while (node != NULL)
        {
            struct name_table_node *next = node->next;
            struct block_id_stack *tmp = node->block_id_stack;
            while (tmp != NULL)
            {
                struct block_id_stack *tmp2 = tmp->next;
                tmp = tmp2;
                free(tmp);
            }
            free(node);
            node = next;
        }
    }    
    free(name_table);
}

/* var_map functions */
int var_map_hash(char *var_name, int block_id)
{
    int result = block_id;
    while (*var_name)
    {
        result += result * 57 + *var_name;
        var_name++;
    }
    return result;
}

struct var_map_node **init_var_map()
{
    return malloc(VAR_TABLE_SZ * sizeof(struct var_map_node*));
}

void var_map_add(struct var_map_node **var_map,
                 char *name, int block_id, enum var_type type, void *pval)
{
    int i = var_map_hash(name, block_id) % VAR_TABLE_SZ;
    struct var_map_node *node = var_map[i];
    if (node == NULL)
    {
        node = malloc(sizeof(struct var_map_node));
        node->name = malloc(strlen(name) + 1);
        strcpy(node->name, name);
        node->block_id = block_id;
        node->elem = NULL;
        node->next = var_map[i];
        var_map[i] = node;
    }
    struct var_map_element *elem = malloc(sizeof(struct var_map_element));
    elem->var_type = type;
    if (elem->var_type == VAR_PROC)
    {
        struct element **p = (struct element**)pval;
        elem->val.proc = *p;
    }
    else if (elem->var_type == VAR_INT)
    {
        int *p = (int*) pval;
        elem->val.num = *p;
    }

    elem->next = node->elem;
    node->elem = elem;
}

struct var_map_element *var_map_get(struct var_map_node** var_map,
                                char *var, int block)
{
    int i = var_map_hash(var, block) % VAR_TABLE_SZ;
    struct var_map_node *node = var_map[i];
    while (node != NULL
          && node->block_id == block
          && strcmp(var, node->name) != 0)
        node = node->next;
    if (node == NULL)
        return NULL;
    return node->elem;
}

/* delete all first from stacks with block id block_id */
void var_map_del_block(struct var_map_node **var_map, int block_id)
{
    for (int i = 0; i < VAR_TABLE_SZ; i++)
    {
        struct var_map_node **pnode = &var_map[i];
        while ((*pnode) != NULL)
        {
            if ((*pnode)->block_id == block_id)
            {
                struct var_map_element *tmp = (*pnode)->elem;
                (*pnode)->elem = tmp->next;
                free(tmp);
            }
            if ((*pnode)->elem == NULL)
            {
                struct var_map_node *tmp = (*pnode);
                *pnode = tmp->next;
                free(tmp->name);
                free(tmp);
            }
            pnode = &(*pnode)->next;
        }
    }
}

void var_map_free(struct var_map_node **var_map)
{
    for (int i = 0; i < VAR_TABLE_SZ; i++)
    {
        struct var_map_node *node = var_map[i];
        while (node != NULL)
        {
            struct var_map_node *next = node->next;
            struct var_map_element *elem = node->elem;
            while (elem != NULL)
            {
                struct var_map_element *tmp = elem;
                elem = elem->next;
                free(tmp);
            }
            free(node->name);
            free(node);
            node = next;
        }
    }    
    free(var_map);
}

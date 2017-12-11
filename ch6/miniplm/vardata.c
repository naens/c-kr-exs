#include <string.h>
#include <stdlib.h>

#include "vardata.h"

#define NAME_TABLE_SZ 16
#define PROC_TABLE_SZ 16

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

/* procedure table functions */
int proc_table_hash(char *var_name, int block_id)
{
    int result = block_id;
    while (*var_name)
    {
        result += result * 57 + *var_name;
        var_name++;
    }
    return result;
}

struct proc_table_node **init_proc_table()
{
    return malloc(PROC_TABLE_SZ * sizeof(struct proc_table_node*));
}

void proc_table_add(struct proc_table_node **proc_table,
                                char *var, int block, struct element *proc)
{
    int i = proc_table_hash(var, block) % PROC_TABLE_SZ;
    struct proc_table_node *node = malloc(sizeof(struct proc_table_node));
    node->next = proc_table[i];
    proc_table[i] = node;
    node->proc = proc;
}

struct element *proc_table_get(struct proc_table_node** proc_table,
                                char *var, int block)
{
    int i = proc_table_hash(var, block) % PROC_TABLE_SZ;
    struct proc_table_node *node = proc_table[i];
    while (node != NULL
          && node->block_id == block
          && strcmp(var, node->name) != 0)
        node = node->next;
    if (node == NULL)
        return NULL;
    return node->proc;
}

void proc_table_free(struct proc_table_node **proc_table)
{
    for (int i = 0; i < PROC_TABLE_SZ; i++)
    {
        struct proc_table_node *node = proc_table[i];
        while (node != NULL)
        {
            struct proc_table_node *next = node->next;
            free(node);
            node = next;
        }
    }    
    free(proc_table);
}

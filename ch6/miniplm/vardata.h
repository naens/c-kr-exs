#ifndef VARDATA_H
#define VARDATA_H

#include "miniplm.h"

/* module for adding data to ast needed during execution */

/* name table: contains stacks for names of variables.
 * each element is a block id, where a variable with such name was declared
 */

struct name_table_node {
    char *var_name;
    struct block_id_stack *block_id_stack;
    struct name_table_node *next;
};

struct block_id_stack {
    int block_id;
    struct block_id_stack *next;
};

struct name_table_node **init_name_table();

void name_table_add(struct name_table_node**, char*, int);

int name_table_get_block(struct name_table_node**, char*);

void name_table_del_block(struct name_table_node**, int);

void name_table_free(struct name_table_node**);


/* procedure table: contains pointers to the ast node of the procedure */

struct proc_table_node {
    char *name;
    int block_id;
    struct element *proc;
    struct proc_table_node *next;
};

struct proc_table_node **init_proc_table();

void proc_table_add(struct proc_table_node**, char*, int, struct element*);

struct element *proc_table_get(struct proc_table_node**, char*, int);

void proc_table_free(struct proc_table_node**);

#endif /* VARDATA_H */

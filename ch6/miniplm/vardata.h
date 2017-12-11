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


/* variable map: block_id,var_name->stack of variables or procedure */

enum var_type {VAR_PROC, VAR_INT};

struct var_map_node {
    char *name;
    int block_id;
    struct var_map_element *elem;
    struct var_map_node *next;
};

struct var_map_element {
    enum var_type var_type;
    union {
        struct element *proc;
        int num;
    } val;
    struct var_map_element *next;
};

struct var_map_node **init_var_map();

void var_map_add(struct var_map_node**, char*, int, enum var_type, void*);

struct var_map_element *var_map_get(struct var_map_node**, char*, int);

void var_map_del_block(struct var_map_node**, int);

void var_map_free(struct var_map_node**);

#endif /* VARDATA_H */

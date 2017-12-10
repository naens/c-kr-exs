#ifndef VARMAP_H
#define VARMAP_H

struct varmap_ht_node {
    char *block_name;
    struct varmap_ht_node *next;
};

struct blockmap_stack {
    struct blockmap_ht_node **blockmap_ht;
    struct blockmap_stack *next;
};

struct blockmap_ht_node {
    char *var_name;
    struct blockmap_ht_node *next;
};

struct var_data {
    int var_type;
    void *value;
};

struct varmap_ht_node **varmap_init();

void blockmap_create(struct varmap_ht_node **varmap, char *block);

void blockmap_delete(struct varmap_ht_node **varmap, char *block);

void var_create(struct varmap_ht_node **varmap, char *var, char *block);

void var_get(struct varmap_ht_node **varmap, char *var, char *block);

#endif /* VARMAP_H */

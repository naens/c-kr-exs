#include <stdio.h>
#include <stdlib.h>

#include "miniplm.h"
#include "parse.h"

void add_to_elem_list(struct element *element, struct element *subelement)
{
    struct elem_list **ppcurr = &element->val.elem_list;
    while (*ppcurr != NULL)
        ppcurr = &(*ppcurr)->next;
    struct elem_list *new_node = malloc(sizeof(struct elem_list));
    new_node->element = subelement;
    new_node->next = *ppcurr;
    *ppcurr = new_node;
}

/* free all allocated structures and unread all elements */
void free_list(FILE *file, struct element *element)
{
    struct elem_list **ppcurr = &element->val.elem_list;
    while (*ppcurr != NULL)
    {
        if ((*ppcurr)->element->elem_term == TERMINAL)
            unread_token(file, (*ppcurr)->element);
        else
            free_list(file, (*ppcurr)->element);
        struct elem_list *tmp = *ppcurr;
        ppcurr = &(*ppcurr)->next;
        free(tmp);
    }
}

/* delete last element from val.elem_list */
void del_last_elem(FILE *file, struct element *element)
{
    struct elem_list **ppcurr = &element->val.elem_list;
    while ((*ppcurr)->next != NULL)
        ppcurr = &(*ppcurr)->next;
    if ((*ppcurr)->element->elem_term == TERMINAL)
        unread_token(file, (*ppcurr)->element);
    else
        free_list(file, (*ppcurr)->element);
    struct elem_list *tmp = *ppcurr;
    *ppcurr = NULL;
    free(tmp);
}

int add_nonterm(FILE *file, struct element *element, 
    int (*pfn)(FILE*, struct element*))
{
    struct element *tmp = malloc(sizeof(struct element));
    if ((*pfn)(file, tmp))
    {
        add_to_elem_list(element, tmp);
        return 1;
    }
    else
    {
        free(tmp);
        return 0;
    }
}

int terminal(FILE *file, struct element *element, enum type t)
{
    int res = read_token(file, element);
    if (res == EOF)
        return 0;
    else if (element->type == t)
        return 1;
    else
    {
        unread_token(file, element);
        return 0;
    }
}

int add_term(FILE *file, struct element *element, enum type t)
{
    struct element *tmp = malloc(sizeof(struct element));
    if (terminal(file, tmp, t))
    {
        add_to_elem_list(element, tmp);
        return 1;
    }
    else
    {
        free(tmp);
        return 0;
    }
}

/* start = module */
int start(FILE *file, struct element *element)
{
    element->type = START;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    /* TODO */
    return 0;
}

/* args = "(" expr { "," expr } ")" */
int args(FILE* file, struct element *element)
{
    element->type = ARGS;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    /* TODO */
    return 0;
}

/* rel_op = "<" | ">" | "<=" | ">=" | "<>" | "=" */
int rel_op(FILE *file, struct element *element)
{
    element->type = REL_OP;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (add_term(file, element, LOWER))
    {
        if (add_term(file, element, GREATER))
            return 1;
        if (add_term(file, element, EQUAL))
            return 1;
        return 1;
    }
    else if (add_term(file, element, GREATER))
    {
        add_term(file, element, EQUAL);
        return 1;
    }
    else if (add_term(file, element, EQUAL))
        return 1;
    return 0;
}

/* expr = arithm [ rel_op arithm ] */
int expr(FILE *file, struct element *element)
{
    element->type = EXPR;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &arithm))
        return 0;
    if (add_nonterm(file, element, &rel_op))
    {
        if (add_nonterm(file, element, &arithm))
            return 1;
        else
        {
            del_last_elem(file, element);
            return 1;
        }
    }
    else
        return 1;
        
}

/* arithm = {term ( "+" | "-" ) } term */
int arithm(FILE* file, struct element *element)
{
    element->type = ARITHM;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &term))
        return 0;
    do {
        if (!add_term(file, element, PLUS) && !add_term(file, element, MINUS))
            return 1;
    } while (add_nonterm(file, element, &term));
    del_last_elem(file, element);
    return 1;
}

/* term = {factor ( "*" | "/" ) } factor */
int term(FILE *file, struct element *element)
{
    element->type = TERM;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &factor))
        return 0;
    do {
        if (!add_term(file, element, STAR) && !add_term(file, element, SLASH))
            return 1;
    } while (add_nonterm(file, element, &factor));
    del_last_elem(file, element);
    return 1;
}

/* factor = [ "-" ] ( number | ident [ args ] | "(" expr ")") */
int factor(FILE *file, struct element *element)
{
    element->type = FACTOR;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    add_term(file, element, MINUS);
    if (add_term(file, element, NUMBER))
        return 1;
    else if (add_term(file, element, IDENT))
    {
        /* TODO: optional args */
        add_nonterm(file, element, &args);
        return 1;
    }
    else if (add_term(file, element, PAROP)
            && add_nonterm(file, element, &expr)
            && add_term(file, element, PARCLOSE))
        return 1;
    else
    {
        free_list(file, element);
        return 0;
    }
}

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
    struct elem_list *pcurr = element->val.elem_list;
    while (pcurr != NULL)
    {
        if (pcurr->element->elem_term == TERMINAL)
            unread_token(file, pcurr->element);
        else
            free_list(file, pcurr->element);
        struct elem_list *tmp = pcurr;
        pcurr = pcurr->next;
        free(tmp);
    }
    element->val.elem_list = NULL;
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
    return add_nonterm(file, element, &module);
}

/* module = ident ":" do_block */
int module(FILE *file, struct element *element)
{
    element->type = MODULE;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, IDENT))
        return 0;
    if (!add_term(file, element, COLON)
        || !add_nonterm(file, element, &do_block))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* declaration = decl_statement | procedure */
int declaration(FILE *file, struct element *element)
{
    element->type = DECLARATION;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &decl_statement)
        && !add_nonterm(file, element, &procedure))
        return 0;
    return 1;
}

/* decl_statement = "DECLARE" decl_element { "," decl_element } ";" */
int decl_statement(FILE *file, struct element *element)
{
    element->type = DECL_STATEMENT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, RW_DECLARE))
        return 0;
    if (!add_nonterm(file, element, &decl_element))
    {
        del_last_elem(file, element);
        return 0;
    }
    while (add_term(file, element, COMMA))
    {
        if (!add_nonterm(file, element, &decl_element))
        {
            free_list(file, element);
            return 0;
        }
    }
    if (!add_term(file, element, SEMICOLON))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* TODO: new decl_element = decl_fact | decl_unfact
 */
/* decl_element = ident type [ initial ] */
int decl_element(FILE *file, struct element *element)
{
    element->type = DECL_ELEMENT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &decl_fact)
        && !add_nonterm(file, element, &decl_unfact))
        return 0;
    return 1;
}

/* add decl_unfact = ident type [ initial ] */
int decl_unfact(FILE *file, struct element *element)
{
    element->type = DECL_UNFACT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, IDENT))
        return 0;
    if (!add_term(file, element, TYPE))
    {
        del_last_elem(file, element);
        return 0;
    }
    add_nonterm(file, element, &initial);
    return 1;
}

/* decl_fact = "(" ident { "," ident } ")" type [ initial ] */
int decl_fact(FILE *file, struct element *element)
{
    element->type = DECL_FACT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, PAROP))
        return 0;
    if (!add_term(file, element, IDENT))
    {
        del_last_elem(file, element);
        return 0;
    }
    while (add_term(file, element, COMMA))
    {
        if (!add_term(file, element, IDENT))
        {
            free_list(file, element);
            return 0;
        }
    }
    if (!add_term(file, element, PARCLOSE))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_term(file, element, TYPE))
    {
        free_list(file, element);
        return 0;
    }
    add_nonterm(file, element, &initial);
    return 1;
}

/* initial = "(" "INITIAL" number ")" */
int initial(FILE *file, struct element *element)
{
    element->type = INITIAL;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, RW_INITIAL))
        return 0;
    if (!add_term(file, element, PAROP))
    {
        del_last_elem(file, element);
        return 0;
    }
    if (!add_term(file, element, NUMBER))
    {
        free_list(file, element);
        return 0;
    }
    add_nonterm(file, element, &params);
    if (!add_term(file, element, PARCLOSE))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* pricedure = proc_statement block_end */
int procedure(FILE *file, struct element *element)
{
    element->type = PROCEDURE;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &proc_statement))
        return 0;
    if (!add_nonterm(file, element, &block_end))
    {
        del_last_elem(file, element);
        return 0;
    }
    return 1;
}

/* proc_statement = ident ":" "PROCEDURE" [ params ] ";" */
int proc_statement(FILE *file, struct element *element)
{
    element->type = PROC_STATEMENT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, IDENT))
        return 0;
    if (!add_term(file, element, COLON))
    {
        del_last_elem(file, element);
        return 0;
    }
    if (!add_term(file, element, RW_PROCEDURE))
    {
        free_list(file, element);
        return 0;
    }
    add_nonterm(file, element, &params);
    add_term(file, element, TYPE);
    if (!add_term(file, element, SEMICOLON))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* params = "(" ident { "," ident } ")" */
int params(FILE *file, struct element *element)
{
    element->type = PARAMS;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, PAROP))
        return 0;
    if (!add_term(file, element, IDENT))
    {
        del_last_elem(file, element);
        return 0;
    }
    while (add_term(file, element, COMMA))
    {
        if (!add_term(file, element, IDENT))
        {
            free_list(file, element);
            return 0;
        }
    }
    if (!add_term(file, element, PARCLOSE))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}


/* unit = cond | do_block | do_while | do_iter | statement */
int unit(FILE *file, struct element *element)
{
    element->type = UNIT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_nonterm(file, element, &cond)
        && !add_nonterm(file, element, &do_block)
        && !add_nonterm(file, element, &do_while)
        && !add_nonterm(file, element, &do_iter)
        && !add_nonterm(file, element, &statement))
        return 0;
    return 1;
}

/* cond = "IF" expr "THEN" unit [ "ELSE" unit ] */
int cond(FILE *file, struct element *element)
{
    element->type = COND;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, RW_IF))
        return 0;
    if (!add_nonterm(file, element, &expr))
    {
        del_last_elem(file, element);
        return 0;
    }
    if (!add_term(file, element, RW_THEN))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &unit))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_term(file, element, RW_ELSE))
        return 1;
    if (!add_nonterm(file, element, &unit))
    {
        del_last_elem(file, element);
        return 1;
    }
    return 1;
}

/* do_block = "DO" ";" block_end */
int do_block(FILE *file, struct element *element)
{
    element->type = DO_BLOCK;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, RW_DO))
        return 0;
    if (!add_term(file, element, SEMICOLON))
    {
        del_last_elem(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &block_end))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* do_while = "DO" "WHILE" expr ";" block_end */
int do_while(FILE *file, struct element *element)
{
    element->type = DO_WHILE;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, RW_DO))
        return 0;
    if (!add_term(file, element, RW_WHILE))
    {
        del_last_elem(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &expr))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_term(file, element, SEMICOLON))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &block_end))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* do_iter = "DO" ident "=" expr "TO" expr [ "BY" expr ] ";" block_end */
int do_iter(FILE *file, struct element *element)
{
    element->type = DO_ITER;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, RW_DO))
        return 0;
    if (!add_term(file, element, IDENT))
    {
        del_last_elem(file, element);
        return 0;
    }
    if (!add_term(file, element, EQUAL))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &expr))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_term(file, element, RW_TO))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &expr))
    {
        free_list(file, element);
        return 0;
    }
    if (add_term(file, element, RW_BY)
        && !add_nonterm(file, element, &expr))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_term(file, element, SEMICOLON))
    {
        free_list(file, element);
        return 0;
    }
    if (!add_nonterm(file, element, &block_end))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
}

/* block_end = { declaration } { unit } "END" [ ident ] ";" */
int block_end(FILE *file, struct element *element)
{
    element->type = BLOCK_END;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    while (add_nonterm(file, element, &declaration))
        ;
    while (add_nonterm(file, element, &unit))
        ;
    if (!add_term(file, element, RW_END))
    {
        free_list(file, element);
        return 0;
    }
    add_term(file, element, IDENT);
    if (!add_term(file, element, SEMICOLON))
    {
        free_list(file, element);
        return 0;
    }
    return 1;    
}

/* statement = ident "=" expr | "CALL" ident [ args ] | ";" | "RETURN" [ expr ] */
int statement(FILE *file, struct element *element)
{
    element->type = STATEMENT;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (add_term(file, element, IDENT))
    {
        if (!add_term(file, element, EQUAL)
            || !add_nonterm(file, element, &expr))
        {
            free_list(file, element);
            return 0;
        }
    }
    else if (add_term(file, element, RW_CALL))
    {
        if (!add_term(file, element, IDENT))
        {
            del_last_elem(file, element);
            return 0;
        }
        add_nonterm(file, element, &args);
    }
    else if (add_term(file, element, RW_RETURN))
        add_nonterm(file, element, &expr);
    if (add_term(file, element, SEMICOLON))
        return 1;
    return 0;
}

/* args = "(" expr { "," expr } ")" */
int args(FILE* file, struct element *element)
{
    element->type = ARGS;
    element->elem_term = NONTERMINAL;
    element->val.elem_list = NULL;
    if (!add_term(file, element, PAROP))
        return 0;

    if (!add_nonterm(file, element, &expr))
    {
        del_last_elem(file, element);
        return 0;
    }
    while (add_term(file, element, COMMA))
    {
        if (!add_nonterm(file, element, &expr))
        {
            free_list(file, element);
            return 0;
        }
    }
    if (!add_term(file, element, PARCLOSE))
    {
        free_list(file, element);
        return 0;
    }
    return 1;
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

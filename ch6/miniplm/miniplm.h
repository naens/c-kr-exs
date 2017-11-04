#ifndef MINIPLM_H
#define MINIPLM_H

#define MAX_IDENTLEN 0x100
#define MAX_QUOTLEN 0x100
#define MAX_NUMLEN 0x100

enum type {
    MODULE, DECLARATION, DECL_STATEMENT, DECL_ELEMENT,
    PROCEDURE, PROC_STATEMENT, PARAMS,
    UNIT, COND, DO_BLOCK, DO_WHILE, DO_ITER, BLOCK_END,
    STATEMENT, ARGS, EXPR, ARITHM, TERM, FACTOR,
    NUMBER, STRING_LIT, IDENT, TYPE,
    PLUS, MINUS, STAR, SLASH, LOWER, GREATER, EQUAL, COLON, SEMICOLON, COMMA,
    PAROP, PARCLOSE,
    RW_DECLARE, RW_PROCEDURE, RW_TO, RW_BY, RW_INITIAL, RW_DO, RW_WHILE, RW_END,
    RW_IF, RW_THEN, RW_ELSE, RW_CALL, RW_RETURN
};

enum elem_term { TERMINAL, NONTERMINAL };

    
char *type[] = { "byte", "integer", "string", NULL };

struct elem_list;

struct element {
    enum type type;
    enum elem_term elem_term;
    union {
        int num;
        char *str;
        struct elem_list *elem_list;
    } val;
};

struct elem_list {
    struct element *element;
    struct elem_list *next;
};

int read_token(FILE*, struct element*);

int sprint_terminal(char*, struct element*);

#endif /* MINIPLM_H */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* predefined strings */
char const *types[] = { "void", "char", "int", "double", NULL };
char const *sspec[] = { "auto", "register", "static", "extern", NULL };
char const *tqual[] = { "const", "volatile", NULL };
char const *tpmod[] = { "signed", "unsigned", NULL };

enum token {
    TOK_IDENT, TOK_NUMBER, TOK_TYPE, TOK_SSPEC, TOK_TQUAL, TOK_TPMOD,
    TOK_PAROP, TOK_PARCLOSE, TOK_BRACKOP, TOK_BRCLOSE, TOK_SEMICOLON,
    TOK_COMMA, TOK_DOT, TOK_STAR,
    TOK_START, TOK_DECL, TOK_DCL, TOK_DDCL, TOK_DSPEC, TOK_PARAM, TOK_PARLIST
};

enum token const stoks[] = { TOK_TYPE, TOK_SSPEC, TOK_TQUAL, TOK_TPMOD};
char **sstrs[] = {
    (char **)types, (char **)sspec, (char **)tqual, (char **)tpmod, NULL
};

enum token_type {
    TTYPE_TERMINAL, TTYPE_NONTERMINAL
};

/* single-token chars */
char const chrs[] = { '(', ')', '[', ']', ';', ',', '.', '*', 0 };
enum token const chrtokens[] = {
    TOK_PAROP, TOK_PARCLOSE, TOK_BRACKOP, TOK_BRCLOSE,
    TOK_SEMICOLON, TOK_COMMA, TOK_DOT, TOK_STAR
};

struct element_list {
    struct element *pe;
    struct element_list *next;
};

/* element, can be a terminal or an ast node */
struct element {
    enum token token;
    enum token_type ttype; /* terminal or non-terminal */
    void *value; /* for terminals: integer (number), NULL or string. */
};

int read_number(char first_digit)
{
    int res = first_digit - '0';
    int c = getchar();
    while (c != EOF && c >= '0' && c <= '9')
    {
        res = res * 10 + c - '0';
        c = getchar();
    }
    if (c != EOF)
        ungetc(c, stdin);
    return res;
}

char *read_str(char first_character)
{
    static char buf[0x100];
    int i = 1;
    buf[0] = first_character;
    int c;
    while ((c = getchar()) != EOF)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                                   || (c == '_') || (c >= '0' && c <= '9'))
            buf[i++] = c;
        else
            break;
    }
    buf[i] = 0;
    char *res = malloc(i + 1);
    strcpy(res, buf);
    if (c != EOF)
        ungetc(c, stdin);
    return res;
}

struct element_list *punread = NULL;

void unread_element(struct element *pe)
{
    struct element_list *new_unread = malloc(sizeof(struct element_list));
    new_unread->pe = pe;
    new_unread->next = punread;
    punread = new_unread;
}

#define READ_OK 0
#define READ_END 1
#define READ_ERR -1
/* struct element must point to a valid struct element element */
int read_token(struct element **p)
{
    if (punread != NULL)		/* read from unreads and return it */
    {
        *p = punread->pe;
        struct element_list *tmp = punread;
        punread = punread->next;
        free(tmp);
        return READ_OK;
    }
    *p = malloc(sizeof(struct element));
    int c = getchar();
    int n_newline = 0;
    while (c == ' ' || c == '\t' || c == '\n')
    {
        if (c == '\n')
            n_newline++;
        if (n_newline > 1)			/* empty line = end of input */
        {
            free(*p);
            return READ_END;
        }
        c = getchar();
    }
    if (c == EOF)			/* end of file? */
    {
        free(*p);
        return READ_END;
    }
    if (c >= '0' && c <= '9')		/* is it a number? */
    {
        (*p)->token = TOK_NUMBER;
        (*p)->ttype = TTYPE_TERMINAL;
        (*p)->value = malloc(sizeof(int));
        *(int*)(*p)->value = read_number(c);
        return READ_OK;
    }
    int i = 0;
    while (chrs[i] != 0)		/* is it a special character? */
    {
        if (chrs[i] == c)
        {
            (*p)->token = chrtokens[i];
            (*p)->ttype = TTYPE_TERMINAL;
            (*p)->value = NULL;
            return READ_OK;
        }
        i++;
    }

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
    {
        char *str = read_str(c);		/* read string */
        int i = 0;
        while (sstrs[i] != NULL)		/* is it a special string? */
        {
            int j = 0;
            while (sstrs[i][j] != 0)
            {
                if (strcmp(sstrs[i][j], str) == 0)
                {
                    (*p)->token = stoks[i];
                    (*p)->ttype = TTYPE_TERMINAL;
                    (*p)->value = str;
                    return READ_OK;
                }
                j++;
            }
            i++;
        }
        (*p)->token = TOK_IDENT;
        (*p)->ttype = TTYPE_TERMINAL;
        (*p)->value = str;
        return READ_OK;
    }
    else
    {
        free(*p);
        return READ_ERR;
    }
}

void print_element(struct element *element);


void print_element_list(struct element_list *pel)
{
    while (pel)
    {
        print_element(pel->pe);
        pel = pel->next;
    }
}

void print_element(struct element *element)
{
    if (element->token == TOK_START)
    {
        printf("[START:");
        print_element_list(element->value);
        printf("]");
    }
    if (element->token == TOK_DECL)
    {
        printf("[DECL:");
        print_element_list(element->value);
        printf("]");
    }
    if (element->token == TOK_DCL)
    {
        printf("[DCL:");
        print_element_list(element->value);
        printf("]");
    }
    if (element->token == TOK_DDCL)
    {
        printf("[DDCL:");
        print_element_list(element->value);
        printf("]");
    }
    if (element->token == TOK_PARAM)
    {
        printf("[PARAM:");
        print_element_list(element->value);
        printf("]");
    }
    if (element->token == TOK_PARLIST)
    {
        printf("[PARLIST:");
        print_element_list(element->value);
        printf("]");
    }
    if (element->token == TOK_DSPEC)
    {
        printf("[DSPEC:");
        print_element_list(element->value);
        printf("]");
    }
    else if (element->token == TOK_NUMBER)
        printf("<number:%d>", *(int*)element->value);
    else if (element->token == TOK_IDENT)
        printf("<identifier:%s>", (char*)element->value);
    else if (element->token == TOK_TYPE)
        printf("<type:%s>", (char*)element->value);
    else if (element->token == TOK_SSPEC)
        printf("<sspec:%s>", (char*)element->value);
    else if (element->token == TOK_TQUAL)
        printf("<tqual:%s>", (char*)element->value);
    else if (element->token == TOK_TPMOD)
        printf("<tomod:%s>", (char*)element->value);
    else
    {
        int i = 0;
        while (chrs[i] != 0)
        {
            if (chrtokens[i] == element->token)
            {
                printf("<%c>", chrs[i]);
                break;
            }
            i++;
        }
    }
}

int dcl(struct element *);
int paramlist(struct element *);

#define P_LIST_END struct element_list **
#define GET_PLIST(e) (struct element_list **) (&((e)->value))

P_LIST_END add_to_end(P_LIST_END p_list_end, struct element *e)
{
    struct element_list *pne = malloc(sizeof(struct element_list));
    pne->pe = e;
    pne->next = NULL;
    *p_list_end = pne;
    return &(pne->next);
}

#define ADD_TO_END(p_list_end,e) p_list_end = add_to_end(p_list_end, e)

void clear_list(void *v)
{
    struct element_list **pel = (struct element_list **) v;
    if (*pel != NULL)
    {
        clear_list(&(*pel)->next);
        if ((*pel)->pe->ttype == TTYPE_TERMINAL)
            unread_element((*pel)->pe);
        else if ((*pel)->pe->ttype == TTYPE_NONTERMINAL)
        {
            clear_list(&(*pel)->pe->value);
            free((*pel)->pe);
        }
       *pel = NULL;
    }
}

int dspec(struct element *e)
{
    e->value = NULL;
    struct element *psub;
    int res = read_token(&psub);
    if (res != READ_OK)
        return 0;
    P_LIST_END ple = GET_PLIST(e);
    if (psub->token == TOK_SSPEC)
    {
        ADD_TO_END(ple, psub);
        res = read_token(&psub);
        if (res != READ_OK)
        {
            clear_list(&e->value);
            return 0;
        }
    }
    if (psub->token == TOK_TPMOD)
    {
        ADD_TO_END(ple, psub);
        res = read_token(&psub);
        if (res != READ_OK)
        {
            clear_list(&e->value);
            return 0;
        }
    }
    if (psub->token == TOK_TYPE)
    {
        e->token = TOK_DSPEC;
        e->ttype = TTYPE_NONTERMINAL;
        ADD_TO_END(ple, psub);
        res = read_token(&psub);
        if (res != READ_OK)
            return 1;
    }
    else
    {
        unread_element(psub);
        clear_list(&e->value);
        return 0;
    }
    if (psub->token == TOK_TQUAL)
        ADD_TO_END(ple, psub);
    else
        unread_element(psub);
    return 1;
}

int ddcl(struct element *e)
{
    /* read{1} <id> OR <(> dcl <)> */
    e->value = NULL;
    struct element *psub;
    int res = read_token(&psub);
    if (res != READ_OK)
        return 0;
    P_LIST_END ple = GET_PLIST(e);
    e->token = TOK_DDCL;
    e->ttype = TTYPE_NONTERMINAL;
    if (psub->token == TOK_IDENT)
    {
        ADD_TO_END(ple, psub);
        res = read_token(&psub);
        if (res != READ_OK)
            return 0;
    }
    else if (psub->token == TOK_PAROP)
    {
        ADD_TO_END(ple, psub);
        struct element *dcl_element = malloc(sizeof(struct element));
        if (!dcl(dcl_element))
        {
            clear_list(&e->value);
            return 0;
        }
        ADD_TO_END(ple, dcl_element);
        res = read_token(&psub);
        if (res != READ_OK)
        {
            clear_list(&e->value);
            return 0;
        }
        if (psub->token == TOK_PARCLOSE)
            ADD_TO_END(ple, psub);
        else
        {
            unread_element(psub);
            clear_list(&e->value);
            return 0;
        }
        res = read_token(&psub);
        if (res != READ_OK)
            return 0;
    }
    /* read{*} <[> <number> <]> OR <(> paramlist <)> */
    if (psub->token != TOK_PAROP && psub->token != TOK_BRACKOP)
    {
        unread_element(psub);
        return 1;
    }
    while (psub->token == TOK_BRACKOP || psub->token == TOK_PAROP)
    {
        if (psub->token == TOK_BRACKOP)
        {
            struct element *op = psub;
            res = read_token(&psub);
            if (res != READ_OK)
            {
                unread_element(op);
                return 1;
            }
            if (psub->token != TOK_NUMBER)
            {
                unread_element(psub);
                unread_element(op);
                return 1;
            }
            struct element *number = psub;
            res = read_token(&psub);
            if (res != READ_OK)
            {
                unread_element(psub);
                unread_element(op);
                return 1;
            }
            if (psub->token == TOK_BRCLOSE)
            {
                ADD_TO_END(ple, op);
                ADD_TO_END(ple, number);
                ADD_TO_END(ple, psub);
            }
            else
            {
                unread_element(psub);
                unread_element(number);
                unread_element(op);
                return 1;
            }
        }
        else if (psub->token == TOK_PAROP)
        {
            struct element *op = psub;
            res = read_token(&psub);
            if (res != READ_OK)
            {
                unread_element(op);
                return 1;
            }
            if (psub->token == TOK_PARCLOSE)
            {
                ADD_TO_END(ple, op);
                ADD_TO_END(ple, psub);
            }
            else
            {
                unread_element(psub);
                struct element *epl = malloc(sizeof(struct element));
                if (!paramlist(epl))
                {
                    unread_element(op);
                    return 1;
                }
                res = read_token(&psub);
                if (res != READ_OK)
                {
                    clear_list(&epl->value);
                    free(epl);
                    unread_element(op);
                    return 1;
                }
                if (psub->token == TOK_PARCLOSE)
                {
                    ADD_TO_END(ple, op);
                    ADD_TO_END(ple, epl);
                    ADD_TO_END(ple, psub);
                }
                else
                {
                    unread_element(psub);
                    clear_list(&epl->value);
                    free(epl);
                    unread_element(op);
                    return 1;
                }
            }
        }

        /* read next element */
        res = read_token(&psub);
        if (res != READ_OK)
            return 1;
        if (psub->token != TOK_PAROP && psub->token != TOK_BRACKOP)
        {
            unread_element(psub);
            return 1;
        }
    }
    return 1;
}

int dcl(struct element *e)
{
    e->token = TOK_DCL;
    e->ttype = TTYPE_NONTERMINAL;
    e->value = NULL;
    struct element *psub;
    P_LIST_END ple = GET_PLIST(e);
    int res = read_token(&psub);
    if (res != READ_OK)
        return 0;
    int has_stars = 0;
    while (psub->token == TOK_STAR)
    {
        ADD_TO_END(ple, psub);
        has_stars = 1;
        res = read_token(&psub);
        if (res != READ_OK)
            return 1;
    }
    unread_element(psub);
    struct element *ddcl_element = malloc(sizeof(struct element));
    if (ddcl(ddcl_element))
    {
        ADD_TO_END(ple, ddcl_element);
        return 1;
    }
    else
    {
        free(ddcl_element);
        if (has_stars)
            return 1;
        else
        {
            clear_list(&e->value);
            return 0;
        }
    }
}
    

int param(struct element *e)
{
    e->value = NULL;
    P_LIST_END ple = GET_PLIST(e);

    struct element *dspec_element = malloc(sizeof(struct element));
    if (!dspec(dspec_element))
    {
        free(dspec_element);
        return 0;
    }
    ADD_TO_END(ple, dspec_element);
    e->token = TOK_PARAM;
    e->ttype = TTYPE_NONTERMINAL;

    struct element *dcl_element = malloc(sizeof(struct element));
    if (!dcl(dcl_element))
    {
        free(dcl_element);		/* accept parameter list with empty dcl */
        return 1;
    }
    ADD_TO_END(ple, dcl_element);

    return 1;
}

int paramlist(struct element *e)
{
    e->value = NULL;
    P_LIST_END ple = GET_PLIST(e);

    /* single param */
    struct element *param_element = malloc(sizeof(struct element));
    if (!param(param_element))
    {
        free(param_element);
        return 0;
    }
    ADD_TO_END(ple, param_element);

    e->token = TOK_PARLIST;
    e->ttype = TTYPE_NONTERMINAL;

    /* now it's already succeded (the rest is optional)
     * only 1 can be returned */

    /* optional multiple <,> param */
    struct element *psub;
    int res = read_token(&psub);
    if (res != READ_OK)
        return 1;
    if (psub->token != TOK_COMMA)
    {
        unread_element(psub);
        return 1;
    }

    struct element *comma;
    struct element *dot;
    while (psub->token == TOK_COMMA)
    {
        comma = psub;
        res = read_token(&psub);
        if (res != READ_OK)
        {
            unread_element(comma);
            return 1;
        }
        if (psub->token == TOK_DOT)
            break;
        unread_element(psub);
        struct element *pel = malloc(sizeof(struct element));
        if (!param(pel))
        {
            unread_element(comma);
            free(pel); 
            return 1;
        }
        ADD_TO_END(ple, comma);
        ADD_TO_END(ple, pel);
        res = read_token(&psub);
        if (res != READ_OK)
            return 1;
    }
    if (psub->token != TOK_DOT)
    {
        unread_element(psub);
        return 1;
    }

    dot = psub;

    /* optional <,> <.> <.> <.> */
    /* here res already contains one <,> and one <.> */
    res = read_token(&psub);
    if (res != READ_OK)
    {
        unread_element(dot);
        unread_element(comma);
        return 1;
    }
    if (psub->token != TOK_DOT)
    {
        unread_element(psub);
        unread_element(dot);
        unread_element(comma);
        return 1;
    }
    
    struct element *dot2 = psub;
    res = read_token(&psub);
    if (res != READ_OK)
    {
        unread_element(dot2);
        unread_element(dot);
        unread_element(comma);
        return 1;
    }
    if (psub->token != TOK_DOT)
    {
        unread_element(psub);
        unread_element(dot2);
        unread_element(dot);
        unread_element(comma);
        return 1;
    }
    ADD_TO_END(ple, comma);
    ADD_TO_END(ple, dot);
    ADD_TO_END(ple, dot2);
    ADD_TO_END(ple, psub);
    return 1;
}

int decl(struct element *e)
{
    e->value = NULL;
    P_LIST_END ple = GET_PLIST(e);

    struct element *dspec_element = malloc(sizeof(struct element));
    if (!dspec(dspec_element))
    {
        free(dspec_element);
        return 0;
    }
    ADD_TO_END(ple, dspec_element);

    struct element *dcl_element = malloc(sizeof(struct element));
    if (!dcl(dcl_element))
    {
        free(dcl_element);
        clear_list(&e->value);
        return 0;
    }
    ADD_TO_END(ple, dcl_element);

    struct element *psub;
    int res = read_token(&psub);
    if (res != READ_OK)
    {
        clear_list(&e->value);
        return 0;
    }
    if (psub->token != TOK_SEMICOLON)
    {
        unread_element(psub);
        clear_list(&e->value);
        return 0;
    }
    ADD_TO_END(ple, psub);

    e->token = TOK_DECL;
    e->ttype = TTYPE_NONTERMINAL;
    return 1;
}

int start(struct element *e)
{
    e->token = TOK_START;
    e->ttype = TTYPE_NONTERMINAL;
    e->value = NULL;
    P_LIST_END ple = GET_PLIST(e);

    struct element *decl_element;
    decl_element = malloc(sizeof(struct element));
    int res;
    int ok = 0;
    while ((res = decl(decl_element)) == 1)
    {
        ADD_TO_END(ple, decl_element);
        decl_element = malloc(sizeof(struct element));
        ok = 1;
    }
    free(decl_element);
    return ok;
}

int main(__attribute__((unused))int argc, __attribute__((unused))char **argv)
{
    int res;
    struct element element;
    res = start(&element);
    print_element(&element);
    printf("\nres=%d\n", res);        
}

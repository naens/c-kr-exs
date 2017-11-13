#include <stdio.h>

#include "miniplm.h"

char spec_from_id(int id)
{
    switch (id)
    {
    case PLUS: return '+';
    case MINUS: return '-';
    case STAR: return '*';
    case SLASH: return '/';
    case LOWER: return '<';
    case GREATER: return '>';
    case EQUAL: return '=';
    case COLON: return ':';
    case SEMICOLON: return ';';
    case COMMA: return ',';
    case PAROP: return '(';
    case PARCLOSE: return ')';
    default: return '?';
    }
    return 0;
}

int is_rw(int type)
{
    return type == RW_DECLARE || type == RW_PROCEDURE
        || type == RW_TO || type == RW_BY || type == RW_INITIAL
        || type == RW_DO || type == RW_WHILE || type == RW_END
        || type == RW_IF || type == RW_THEN || type == RW_ELSE
        || type == RW_CALL || type == RW_RETURN;
}

int sprint_rw(char *buf, int type)
{
    int n = 0;
    switch (type)
    {
    case RW_DECLARE:
        n = sprintf(buf, "declare");
        break;
    case RW_PROCEDURE:
        n = sprintf(buf, "procedure");
        break;
    case RW_TO:
        n = sprintf(buf, "to");
        break;
    case RW_BY:
        n = sprintf(buf, "by");
        break;
    case RW_INITIAL:
        n = sprintf(buf, "initial");
        break;
    case RW_DO:
        n = sprintf(buf, "do");
        break;
    case RW_WHILE:
        n = sprintf(buf, "while");
        break;
    case RW_END:
        n = sprintf(buf, "end");
        break;
    case RW_IF:
        n = sprintf(buf, "if");
        break;
    case RW_THEN:
        n = sprintf(buf, "then");
        break;
    case RW_ELSE:
        n = sprintf(buf, "else");
        break;
    case RW_CALL:
        n = sprintf(buf, "call");
        break;
    case RW_RETURN:
        n = sprintf(buf, "return");
        break;
    }
    return n;
}

int sprint_elem_json(char *buf, struct element *element, int depth)
{
    int n = 0;
    if (depth == 0)
    {
        n += sprintf(buf + n, "\"");
        n += sprint_elem_str(buf + n, element);
        n += sprintf(buf + n, "\"");
        return n;
    }
    if (element->elem_term == TERMINAL)
    {
        if (element->type == NUMBER)
            n = sprintf(buf, "{\"number\":\"%d\"}", element->val.num);
        else if (element->type == TYPE)
            n = sprintf(buf, "{\"type\":\"%s\"}", element->val.str);
        else if (element->type == STRING_LIT)
            n = sprintf(buf, "{\"quote\":\"%s\"}", element->val.str);
        else if (is_rw(element->type))
        {
            n += sprintf(buf + n, "\"");
            n += sprint_rw(buf + n, element->type);
            n += sprintf(buf + n, "\"");
        }
        else if (element->type == IDENT)
            n = sprintf(buf, "{\"ident\":\"%s\"}", element->val.str);
        else
            n = sprintf(buf, "\"%c\"", spec_from_id(element->type));
    }
    else
    {
        n += sprintf(buf + n, "{");
        switch (element->type)
        {
        case START:
            n += sprintf(buf + n, "\"start\"");
            break;
        case MODULE:
            n += sprintf(buf + n, "\"module\"");
            break;
        case DECLARATION:
            n += sprintf(buf + n, "\"declaration\"");
            break;
        case DECL_STATEMENT:
            n += sprintf(buf + n, "\"decl_statement\"");
            break;
        case DECL_ELEMENT:
            n += sprintf(buf + n, "\"decl_element\"");
            break;
        case DECL_UNFACT:
            n += sprintf(buf + n, "\"decl_unfact\"");
            break;
        case DECL_FACT:
            n += sprintf(buf + n, "\"decl_fact\"");
            break;
        case INITIAL:
            n += sprintf(buf + n, "\"initial\"");
            break;
        case PROCEDURE:
            n += sprintf(buf + n, "\"procedure\"");
            break;
        case PROC_STATEMENT:
            n += sprintf(buf + n, "\"proc_statement\"");
            break;
        case PARAMS:
            n += sprintf(buf + n, "\"params\"");
            break;
        case UNIT:
            n += sprintf(buf + n, "\"unit\"");
            break;
        case COND:
            n += sprintf(buf + n, "\"cond\"");
            break;
        case DO_BLOCK:
            n += sprintf(buf + n, "\"do_block\"");
            break;
        case DO_WHILE:
            n += sprintf(buf + n, "\"do_while\"");
            break;
        case DO_ITER:
            n += sprintf(buf + n, "\"do_iter\"");
            break;
        case BLOCK_END:
            n += sprintf(buf + n, "\"block_end\"");
            break;
        case STATEMENT:
            n += sprintf(buf + n, "\"statement\"");
            break;
        case ARGS:
            n += sprintf(buf + n, "\"args\"");
            break;
        case REL_OP:
            n += sprintf(buf + n, "\"rel_op\"");
            break;
        case EXPR:
            n += sprintf(buf + n, "\"expr\"");
            break;
        case ARITHM:
            n += sprintf(buf + n, "\"arithm\"");
            break;
        case TERM:
            n += sprintf(buf + n, "\"term\"");
            break;
        case FACTOR:
            n += sprintf(buf + n, "\"factor\"");
            break;
        default:
            n += sprintf(buf + n, "\"nonterminal\"");
            break;
        }
        n += sprintf(buf + n, ":[");
        struct elem_list *plist = element->val.elem_list;
        int first = 1;
        while (plist != NULL)
        {
            if (!first)
                n += sprintf(buf + n, ",");
            n += sprint_elem_json(buf + n, plist->element, depth - 1);
            plist = plist->next;
            first = 0;
        }
        n += sprintf(buf + n, "]}");
    }
    return n;
}

int sprint_elem_str(char *buf, struct element *element)
{
    int n = 0;
    if (element->elem_term == TERMINAL)
    {
        if (element->type == NUMBER)
            n += sprintf(buf, "%d", element->val.num);
        else if (element->type == TYPE || element->type == STRING_LIT
            || element->type == IDENT)
            n += sprintf(buf, "%s", element->val.str);
        else if (is_rw(element->type))
        {
            n += sprint_rw(buf + n, element->type);
//            n += sprintf(buf + n, " ");
        }
        else
            n += sprintf(buf, "%c", spec_from_id(element->type));
    }
    else
    {
        struct elem_list *p = element->val.elem_list;
        int first = 1;
        while (p != NULL)
        {
            if (!first)
                n += sprintf(buf + n, " ");
            n += sprint_elem_str(buf + n, p->element);
            p = p->next;
            first = 0;
        }
    }
    return n;   
}

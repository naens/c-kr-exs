#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "miniplm.h"

int is_spec(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/'
        || c == '<' || c == '>' || c == '='
        || c == ':' || c == ';' || c == ','
        || c == '(' || c == ')';
}

int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

enum type get_spec(char c)
{
    switch (c)
    {
    case '+': return PLUS;
    case '-': return MINUS;
    case '*': return STAR;
    case '/': return SLASH;
    case '<': return LOWER;
    case '>': return GREATER;
    case '=': return EQUAL;
    case ':': return COLON;
    case ';': return SEMICOLON;
    case ',': return COMMA;
    case '(': return PAROP;
    case ')': return PARCLOSE;
    }
    return -1;
}

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

int is_num(char c)
{
    return (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F')
        || (c >= '0' && c <= '9');
}

void read_number(FILE *file, char first, int *num)
{
    char buf[MAX_NUMLEN];
    char *pbuf = buf;
    *pbuf++ = first;
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (is_num(c))
            *pbuf++ = c;
        else if (c == '$')
            continue;
        else
            break;
    }
    *pbuf = 0;
    if (c == 'h' || c == 'H')
        *num = strtol(buf, NULL, 16);
    else if (c == 'o' || c == 'O' || c == 'q' || c == 'Q')
        *num = strtol(buf, NULL, 8);
    else
    {
        ungetc(c, file);
        char prevc = *(pbuf - 1);
        if (prevc == 'd' || prevc == 'D')
            *num = strtol(buf, NULL, 10);
        else if (prevc == 'b' || prevc == 'B')
            *num = strtol(buf, NULL, 2);
        else
        {
            *num = strtol(buf, NULL, 10);
        }
    }
}

int is_alphanum(char c)
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9');
}

void read_string(FILE *file, char first, char *buf)
{
    *buf++ = first;
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (is_alphanum(c))
            *buf++ = c;
        else if (c == '$')
            continue;
        else
            break;
    }
    ungetc(c, file);
    *buf = '\0';
}

int acontains(char **a, char *s)
{
    while (*a != NULL)
    {
        if (strcmp(s, *a) == 0)
            return 1;
        a++;
    }
    return 0;
}

int get_reserved(char *str, int *is_reserved)
{
    *is_reserved = 1;
    if (strcmp(str, "declare") == 0)
        return RW_DECLARE;
    else if (strcmp(str, "procedure") == 0)
        return RW_PROCEDURE;
    else if (strcmp(str, "to") == 0)
        return RW_TO;
    else if (strcmp(str, "by") == 0)
        return RW_BY;
    else if (strcmp(str, "initial") == 0)
        return RW_INITIAL;
    else if (strcmp(str, "do") == 0)
        return RW_DO;
    else if (strcmp(str, "while") == 0)
        return RW_WHILE;
    else if (strcmp(str, "end") == 0)
        return RW_END;
    else if (strcmp(str, "if") == 0)
        return RW_IF;
    else if (strcmp(str, "then") == 0)
        return RW_THEN;
    else if (strcmp(str, "else") == 0)
        return RW_ELSE;
    else if (strcmp(str, "call") == 0)
        return RW_CALL;
    else if (strcmp(str, "return") == 0)
        return RW_RETURN;
    *is_reserved = 0;
    return -1;
}

void read_quoted(FILE *file, char *buf)
{
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\'')
        {
            c = fgetc(file);
            if (c == EOF)
                break;
            else if (c == '\'')
                *buf++ = c;
            else
            {
                ungetc(c, file);
                break;
            }
        }
        else
            *buf++ = c;
    }
    *buf = 0;
}

/* Reads a single token from file, unreads if character 
 * is read outside the token.
 * Spaces and comments are skipped
 * element can be of type:
 * + type
 * + other reserved word
 * + string
 * + number
 * + special character 
 * return value: 
 * + 0: "everything ok"
 * + EOF: end of input, means no token was read
 */
int read_token(FILE *file, struct element *element)
{
    int c;
    int is_comment = 0;
    element->elem_term = TERMINAL;
    element->val.str = NULL;
    while ((c = fgetc(file)) != EOF)
    {
        if (is_comment)
        {
            if (c == '*')
            {
                c = fgetc(file);
                if (c == EOF)      /* comment without ending */
                    return EOF;
                else if (c == '/') /* end of comment */
                {
                    is_comment = 0;
                    continue;
                }
            }
            else
                continue;
        }
        else if (c == '/')
        {
            c = fgetc(file);
            if (c == '*')
            {
                is_comment = 1;
                continue;
            }
            else
            {
                ungetc(c, file); /* even if c is EOF it should work */
                element->type = SLASH;
                return 0;
            }
        }
        else if (is_space(c))
            continue;
        else if (is_spec(c))
        {
            element->type = get_spec(c);
            return 0;
        }
        else if (c >= '0' && c <= '9')
        {
            int value;
            read_number(file, c, &value);
            element->type = NUMBER;
            element->val.num = value;
            return 0;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            char buf[MAX_IDENTLEN];
            read_string(file, c, buf);
            int len = strlen(buf);
            char *value = malloc(len + 1);
            strcpy(value, buf);
            int is_reserved;
            int token_type = get_reserved(buf, &is_reserved);
            if (is_reserved)
                element->type = token_type;
            else
            {
                token_type = (acontains(type, value)) ? TYPE : IDENT;
                element->type = token_type;
                element->val.str = value;
            }
            return 0;
        }
        else if (c == '\'')
        {
            char buf[MAX_QUOTLEN];
            read_quoted(file, buf);
            int len = strlen(buf);
            char *value = malloc(len + 1);
            strcpy(value, buf);
            element->type = STRING_LIT;
            element->val.str = value;

            return 0;            
        }
    }
    return EOF;
}

int sprint_terminal(char *buf, struct element *element)
{
    int n = 0;
    if (element->elem_term == TERMINAL)
    {
        switch (element->type)
        {
        case NUMBER:
            n = sprintf(buf, "<number:%d>", element->val.num);
            break;
        case TYPE:
            n = sprintf(buf, "<type:%s>", element->val.str);
            break;
        case STRING_LIT:
            n = sprintf(buf, "<quote:%s>", element->val.str);
            break;
        case RW_DECLARE:
            n = sprintf(buf, "<declare>");
            break;
        case RW_PROCEDURE:
            n = sprintf(buf, "<procedure>");
            break;
        case RW_TO:
            n = sprintf(buf, "<to>");
            break;
        case RW_BY:
            n = sprintf(buf, "<by>");
            break;
        case RW_INITIAL:
            n = sprintf(buf, "<initial>");
            break;
        case RW_DO:
            n = sprintf(buf, "<do>");
            break;
        case RW_WHILE:
            n = sprintf(buf, "<while>");
            break;
        case RW_END:
            n = sprintf(buf, "<end>");
            break;
        case RW_IF:
            n = sprintf(buf, "<if>");
            break;
        case RW_THEN:
            n = sprintf(buf, "<then>");
            break;
        case RW_ELSE:
            n = sprintf(buf, "<else>");
            break;
        case RW_CALL:
            n = sprintf(buf, "<call>");
            break;
        case RW_RETURN:
            n = sprintf(buf, "<return>");
            break;
        case IDENT:
            n = sprintf(buf, "<ident:%s>", element->val.str);
            break;
        default:
            n = sprintf(buf, "<%c>", spec_from_id(element->type));
            break;
        }
    }
    return n;
}

int main(int argc, char **argv)
{
    FILE *f;
    if (argc == 2)
    {
        f = fopen(argv[1], "r");
        if (f == NULL)
        {
            fprintf(stderr, "could not open %s\n", argv[1]);
            return 1;
        }
    }
    else
        f = stdin;

    int res;
    struct element element;
    while ((res = read_token(f, &element)) != EOF)
    {
        char buf[0x100];
        sprint_terminal(buf, &element);
        printf("%s\n", buf);
    }

    printf("\n");
    return 0;
}

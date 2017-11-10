#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "miniplm.h"
#include "parse.h"

int (*name_to_pfun(char *fname))(FILE*, struct element*)
{
    if (strcmp(fname, "start") == 0)
        return &start;
    else if (strcmp(fname, "module") == 0)
        return &module;
    else if (strcmp(fname, "declaration") == 0)
        return &declaration;
    else if (strcmp(fname, "decl_statement") == 0)
        return &decl_statement;
    else if (strcmp(fname, "decl_element") == 0)
        return &decl_element;
    else if (strcmp(fname, "initial") == 0)
        return &initial;
    else if (strcmp(fname, "procedure") == 0)
        return &procedure;
    else if (strcmp(fname, "proc_statement") == 0)
        return &proc_statement;
    else if (strcmp(fname, "params") == 0)
        return &params;
    else if (strcmp(fname, "unit") == 0)
        return &unit;
    else if (strcmp(fname, "cond") == 0)
        return &cond;
    else if (strcmp(fname, "do_block") == 0)
        return &do_block;
    else if (strcmp(fname, "do_while") == 0)
        return &do_while;
    else if (strcmp(fname, "do_iter") == 0)
        return &do_iter;
    else if (strcmp(fname, "block_end") == 0)
        return &block_end;
    else if (strcmp(fname, "statement") == 0)
        return &statement;
    else if (strcmp(fname, "args") == 0)
        return &args;
    else if (strcmp(fname, "rel_op") == 0)
        return &rel_op;
    else if (strcmp(fname, "expr") == 0)
        return &expr;
    else if (strcmp(fname, "arithm") == 0)
        return &arithm;
    else if (strcmp(fname, "term") == 0)
        return &term;
    else if (strcmp(fname, "factor") == 0)
        return &factor;
    else return NULL;
 }

/* usage:
    * <file>
    * <start> {<input>}
 */
int main(int argc, char **argv)
{
    FILE *file;
    char *fstr = NULL;
    int (*pstart)(FILE*, struct element*);
    if (argc == 2)		/* argv[1]: file, start from start */
    {
        file = fopen(argv[1], "r");
        if  (file == NULL)
        {
            fprintf(stderr, "error opening file '%s'\n", argv[1]);
            return 1;
        }
        pstart = &start;
    }
    else if (argc >= 3)		/* start from argv[1], file from rest */
    {
        char *fname = argv[1];
        pstart = name_to_pfun(fname);
        if (pstart == NULL)
        {
            fprintf(stderr, "bad function name: '%s'\n", fname);
            return 2;
        }
        int len = 0;
        char **ps = &argv[2];
        while (*ps != NULL)
            len += 1 + strlen(*(ps++));
        fstr = malloc(len);
        char *s = fstr;
        char *t;
        for (int i = 2; i < argc; i++)
        {
            t = argv[i];
            while (*t != '\0')
                *s++ = *t++;
            if (i < argc - 1)
                *s++ = ' ';
        }
        *s = 0;
//        printf("input string: '%s', starting from %s\n", fstr, fname);
        file = fmemopen(fstr, len, "r");
        if (file == NULL)
        {
            fprintf(stderr, "fmemopen error\n");
            return 5;
        }
    }
    else
    {
        fprintf(stderr, "usage:\n\t<file>\n or\t<start function> {<input>}\n\n");
        return 3;
    }
    struct element element;
    int res = (*pstart)(file, &element);
    if (!res)
    {
        fprintf(stderr, "\"bad input\"\n");
        return 7;
    }

    char buf[0x10000];
    sprint_elem_json(buf, &element, 2);
    printf("%s\n", buf);

    if (fstr != NULL)
        free(fstr);
    
    return 0;
}

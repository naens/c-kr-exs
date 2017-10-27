#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define IS_D(x) ((x) >= 0 && (x) <= 9 \
            || (x) >= 'a' && (x) <= 'z'\
            || (x) >= 'A' && (x) <= 'Z'\
            || (x) == ' ' || (x) == '\t' || (x) == '\n')

#define CH_CMP(c1,c2,f) (f ? tolc(c1) - tolc(c2) : c1 - c2)

#define MAX_LINE 0x1000
#define NLINES 0x100

int ncmp(char *s1, char *s2)
{
    double d1 = atof(s1);
    double d2 = atof(s2);
    return d1 > d2 ? 1 : d1 < d2 ? -1 : 0;
}

int tolc(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

int scmp(char *s1, char *s2, int f)
{
    while (CH_CMP(*s1, *s2, f) == 0)
        s1++, s2++;
    return CH_CMP(*s1, *s2, f);
}

int dcmp(char *s1, char *s2, int f)
{
    while (*s1 && *s2)
    {
        if (!IS_D(*s1))
            s1++;
        else if (!IS_D(*s2))
            s2++;
        else if (CH_CMP(*s1, *s2, f) == 0)
            s1++, s2++;
        else
            break;
    }
    return CH_CMP(*s1, *s2, f);
}

int cmp1(char *str1, char *str2, int n, int r, int f, int d)
{
    int res;
    if (n)
        res = ncmp(str1, str2);
    else if (d)
        res = dcmp(str1, str2, f);
    else
        res = scmp(str1, str2, f);
    return r ? -res : res;    
}

void get_fields(char **fields, int i, int *n, int *r, int *f, int *d)
{
    *n = 0;
    *r = 0;
    *f = 0;
    *d = 0;
    int len = 0;
    while (fields[len])
        len++;
    if (i >= len)
        return;
    char *field_string = fields[i];
    while (*field_string)
    {
        switch (*field_string)
        {
        case 'n':
            *n = 1;
            break;
        case 'r':
            *r = 1;
            break;
        case 'f':
            *f = 1;
            break;
        case 'd':
            *d = 1;
            break;
        }
        field_string++;
    }
}

/* fields array contains for each field flags (n,r,f,d)
 * n: use number comparator
 * r: reverse comparison result
 * f: convert to lowerrcase before comparison
 * d: for comparison skip characters different from 0-9, a-z, A-Z, ' ', '\t', '\n'
 */
int field_cmp(char *str1, char *str2, char **fields)
{
    char *saveptr1;
    char *saveptr2;
    
    char as1[strlen(str1) + 1];
    char as2[strlen(str2) + 1];
    strcpy(as1, str1);
    strcpy(as2, str2);
    char *s1 = strtok_r(as1, " ", &saveptr1);
    char *s2 = strtok_r(as2, " ", &saveptr2);

    int n, r, f, d;

    int i = 0;
    while (s1 != NULL && s2 != NULL)
    {
        get_fields(fields, i, &n, &r, &f, &d);
        int res = cmp1(s1, s2, n, r, f, d);
        if (res)
            return res;
        s1 = strtok_r(NULL, " ", &saveptr1);
        s2 = strtok_r(NULL, " ", &saveptr2);
        i++;
    }
}

int line(char *buf)
{
    int c;
    int i = 0;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            buf[i] = 0;
            return 0;
        }
        else
        {
            buf[i] = c;
            i++;
        }
    }
    return EOF;
}

int main(int argc, char **argv)
{
    char *fields[argc - 1];
    for(int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            fields[i - 1] = &argv[i][1];
        else
        {
            fprintf(stderr, "bad arguments\n");
            return 1;
        }
    }
    char input[NLINES][MAX_LINE];
    int sz = 0;
    int res;
    while ((res = line(input[sz])) != EOF && sz < NLINES)
    {
        if (strlen(input[sz]) == 0)
            break;
        sz++;
    }

    qsort_r(input, sz, MAX_LINE, (__compar_d_fn_t)&field_cmp, fields);

    for (int i = 0; i < sz; i++)
        printf("%s\n", input[i]);
    return 0;
}

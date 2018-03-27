#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

typedef uint64_t tbr;
typedef tbr* tbl;
#define B_SIZE(b) *((tbl)(b))
#define B_NEXT(b) (tbl)(*((tbl)(b)+1))
#define B_PREV(b) (tbl)(*((tbl)(b)+2))
#define B_DATA(b) ((tbl)(b)+3)
#define B_FROM_DATA_PTR(p) ((tbl)(p) - 3)
#define B_SETSIZE(b,s) (*((tbl)(b)) = (s))
#define B_SETNEXT(b,n) (*((tbl)(b) + 1) = ((tbr)(n)))
#define B_SETPREV(b,n) (*((tbl)(b) + 2) = ((tbr)(n)))
#define B_NBLOCKS(n_bytes) ((((tbr)(n_bytes)) + sizeof(tbl)) / sizeof(tbl))

#define BUF_SZ 0x1000
#define MIN_ADD_SZ 0x20 /* minimum amount of blocks to add */
#define MIN_DATA_SZ 0x8 /* minimum size of a block, don't split further */

tbl pbuf[BUF_SZ];
tbl buf = (tbl) pbuf;
tbr used;

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

void init()
{
    if (sizeof (tbr) != sizeof (tbl))
        error("type tbr and tbl do not have the same size\n"
            "sizeof(tbr) = %d and sizeof(tbl) = %d", sizeof (tbr), sizeof (tbl));
    B_SETSIZE(buf, 0);
    B_SETPREV(buf, buf);
    B_SETNEXT(buf, buf);
    used = 3;
}

/* insert b2 before b1:  pprev->b1 => pprev->b2->b1 */
void insert_before(tbl b1, tbl b2)
{
    tbl pprev = B_PREV(b1);
    B_SETNEXT(b2, b1);
    B_SETPREV(b1, b2);
    B_SETNEXT(pprev, b2);
    B_SETPREV(b2, pprev);
}

/* insert b2 after b1:  b1->nnext => b1->b2->nnext */
void insert_after(tbl b1, tbl b2)
{
    tbl nnext = B_NEXT(b1);
    B_SETNEXT(b1, b2);
    B_SETPREV(b2, b1);
    B_SETNEXT(b2, nnext);
    B_SETPREV(nnext, b2);
}

/* removes b from the list */
void del(tbl b)
{
    if (b == buf)
        return;
    tbl prev = B_PREV(b);
    tbl next = B_NEXT(b);
    B_SETNEXT(prev, next);
    B_SETPREV(next, prev);
}

/* add memory to current memory, returns index of new segment
 * the memory is initialized */
tbl add_mem(tbr nblocks)
{
    if (nblocks < MIN_ADD_SZ)
        nblocks = MIN_ADD_SZ;

    if (used + nblocks >= BUF_SZ)
        return 0;

    tbl result = (tbl)(&pbuf[used]);
    used += nblocks + 3;
    B_SETSIZE(result, nblocks);
    insert_before(buf, result);
    return result;
}

/* get data from list, split and remove if needed
 * returns pointer to data */
void *take(tbl b, tbr sz)
{
    void *data;
    data = (void*)B_DATA(b);
    if (B_SIZE(b) >= MIN_DATA_SZ + 3 + sz)
    {
        tbl bnew = b + 3 + sz;
        B_SETSIZE(bnew, B_SIZE(b) - sz - 3);
        B_SETSIZE(b, sz);
        insert_after(b, bnew);
    }
    del(b);
    return data;
}

void *get_mem(int n_bytes)
{
    tbr sz = B_NBLOCKS(n_bytes);

    tbl curr = B_NEXT(buf);
    while (curr != buf)
    {
        tbr csz = B_SIZE(curr);
        if (csz >= sz)
            return take(curr, sz);
        curr = B_NEXT(curr);
    }

    if (curr == buf)
    {
        curr = add_mem(sz);
        if (curr == 0)
            return NULL;
        return take(curr, sz);
    }
}

void try_merge(tbl b1, tbl b2)
{
    if (b1 == buf)
        return;

    if (b2 == buf)
        return;

    tbr sz1 = B_SIZE(b1);
    if (B_DATA(b1) + sz1 == b2)
    {
        B_SETSIZE(b1, sz1 + B_SIZE(b2) + 3);
        del(b2);
    }
}

/* try to merge with previous and next element */
void check_merge(tbl b)
{
    tbl prev = B_PREV(b);
    try_merge(prev, b);
    tbl next = B_NEXT(b);
    try_merge(b, next);
}

tbl find_next(tbl b)
{
    tbl curr = B_NEXT(buf);
    while (B_DATA(curr) < b && curr != buf)
        curr = B_NEXT(curr);
    return curr;
}

void free_mem(void *p)
{
    tbl pp = (tbl)p;
    tbl b = B_FROM_DATA_PTR(pp);
    tbl next = find_next(b);
    insert_before(next, b);
    check_merge(b);
}

void print_free_list()
{
    tbl curr = B_NEXT(buf);
    printf(" used:%d; ", used);
    if (curr == buf)
        printf("empty");
    while (curr != buf)
    {
        printf("%d,%08x ", B_SIZE(curr), ((uint64_t)B_NEXT(curr)) & 0xffffffff);
        curr = B_NEXT(curr);
    }
    printf("\n");
}

/*
 * tested by executing ./exmem $((RANDOM % 1000))
 */
int main(int argc, char **argv)
{
    init();
    const int AR_SZ = 8;
    char *ar[AR_SZ];
    int item;
    int n;
    int seed;
    if (argc == 2)
    	seed = atoi(argv[1]);
    else
    	seed = 0;
    srand(seed);
    for (int i = 0; i < AR_SZ; i++)
    	ar[i] = NULL;
    for (int i = 0; i < AR_SZ * 3; i++) {
    	item = rand() % AR_SZ;
    	if (ar[item] == NULL) {
    	    n = rand() % (2 * BUF_SZ);
    	    printf("ar[%d] = get_mem(%d);	//", item, n);
    	    ar[item] = get_mem(n);
    	    if (ar[item] == NULL)
    	    	printf(" NO MEM");
    	} else {
    	    printf("free_mem(ar[%d]);	//", item);
    	    free_mem(ar[item]);
    	    ar[item] = NULL;
    	}
    	print_free_list();
    }
    return 0;
}


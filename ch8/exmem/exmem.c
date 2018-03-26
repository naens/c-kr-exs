#include <stdio.h>
#include <stdint.h>

typedef uint64_t tbl;
#define B_ADD(p, n) ((p) + (n) * sizeof(tbl))
#define B_ACCESS(b, n) (*(((tbl*)b) + n))
#define B_SIZE(b) B_ACCESS(b, 0)
#define B_NEXT(b) B_ACCESS(b, 1)
#define B_PREV(b) B_ACCESS(b, 2)
#define B_DATA(b) B_ADD(b, 3)
#define B_FROM_DATA_PTR(p) B_ADD(p, -3)
#define B_SETSIZE(b,s) (B_ACCESS(b, 0) = (s))
#define B_SETNEXT(b,n) (B_ACCESS(b, 1) = (n))
#define B_SETPREV(b,n) (B_ACCESS(b, 2) = (n))
#define B_NBLOCKS(n_bytes) ((((uint64_t)(n_bytes)) + sizeof(tbl)) / sizeof(tbl))

#define BUF_SZ 0x1000
#define MIN_ADD_SZ 0x20 /* minimum amount of blocks to add */
#define MIN_DATA_SZ 0x8 /* minimum size of a block, don't split further */

tbl pbuf[BUF_SZ];
tbl buf = (tbl) pbuf;
tbl used;

void init()
{
    B_SIZE(buf) = 0;
    B_PREV(buf) = buf;
    B_NEXT(buf) = buf;
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
tbl add_mem(tbl nblocks)
{
    if (nblocks < MIN_ADD_SZ)
        nblocks = MIN_ADD_SZ;

    if (used + nblocks < BUF_SZ)
        return 0;

    tbl result = (tbl)(&pbuf[used]);
    used += B_ADD(nblocks, 3); /* add for the header */
    B_SETSIZE(result, nblocks);
    insert_before(buf, result);
    return result;
}

/* get data from list, split and remove if needed
 * returns pointer to data */
void *take(tbl b, tbl sz)
{
    void *data;
    tbl new_sz = B_SIZE(b) - sz - 3;
    data = (void*)B_DATA(b);
    if (new_sz >= MIN_DATA_SZ)
    {
        tbl bnew = B_ADD(b, 3 + sz);
        B_SETSIZE(b, sz);
        insert_after(b, bnew);
    }
    del(b);
    return data;
}

void *get_mem(int n_bytes)
{
    tbl sz = B_NBLOCKS(n_bytes);
    tbl data;

    /* try to find a block */
    tbl curr = B_NEXT(buf);
    while (curr != buf)
    {
        int csz = B_SIZE(curr);
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

    tbl sz1 = B_SIZE(b1);
    if (B_ADD(B_DATA(b1), sz1) == b2)
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
    try_merge(next, b);
}

tbl find_next(tbl b)
{
    tbl curr = B_NEXT(buf);
    while (B_DATA(curr) < b && curr != buf)
        curr = B_NEXT(curr);
    return curr == buf ? 0 : curr;
}

void free_mem(void *p)
{
    tbl pp = (tbl)p;
    tbl b = B_FROM_DATA_PTR(pp);
    insert_before(b, find_next(b));
    check_merge(b);
}

int main(int argc, char **argv)
{
    init();
    return 0;
}

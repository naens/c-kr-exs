#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HSZ 11

struct tbl_node {
  int key;
  int val;
  struct tbl_node *next;
};

int hash(int key)
{
  return key * 3;
}

struct tbl_node **find_node_ptr(struct tbl_node **tbl, int key)
{
  //  printf("FIND\n");
  struct tbl_node **node_ptr = &tbl[hash(key) % HSZ];
  while ((*node_ptr) != NULL
	 && (*node_ptr)->key != key)
    node_ptr=&((*node_ptr)->next);
  return node_ptr;
}

void tbl_set(struct tbl_node **tbl, int key, int val)
{
  //  printf("SET\n");
  struct tbl_node **node_ptr = find_node_ptr(tbl, key);
  if (*node_ptr == NULL)
    {
      struct tbl_node *node = malloc(sizeof(struct tbl_node));
      node->key = key;
      node->val = val;
      node->next = NULL;
      *node_ptr = node;	
    }
  else
    (*node_ptr)->val = val;
}

int tbl_get(struct tbl_node **tbl, int key)
{
  //  printf("GET\n");
  struct tbl_node **node_ptr = find_node_ptr(tbl, key);
  if (*node_ptr == NULL)
    return 0;
  else
    return (*node_ptr)->val;
}

void tbl_clear(struct tbl_node **tbl)
{
  struct tbl_node *n;
  struct tbl_node *m;
  for (int i = 0; i < HSZ; i++)
    {
      n = tbl[i];
      while (n != NULL)
	{
	  m = n->next;
	  free(n);
	  n = m;
	}
    }
}

int main()
{
  struct tbl_node *tbl[HSZ];
  memset(tbl, 0, HSZ * (sizeof(struct tbl *)));

  int c;
  int min_chr = -1;
  int max_chr = -1;
  while ((c = getchar()) != EOF)
    {
      if (min_chr == -1 || c < min_chr)
	min_chr = c;
      if (max_chr == -1 || c > max_chr)
	max_chr = c;
      tbl_set(tbl, c, tbl_get(tbl, c) + 1);
    }

  for (int i = min_chr; i <= max_chr; i++)
    {
      int freq = tbl_get(tbl, i);
      if (freq > 0)
	{
	  printf("0x%02x (%c) #", i, (i >= ' ' && i < 0xff) ? i : ' ');
	  for (int j = 0; j < freq; j++)
	    putchar('-');
	  printf(">\n");
	}
    }

  tbl_clear(tbl);
}

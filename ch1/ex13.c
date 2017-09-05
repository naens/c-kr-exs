#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HT_SZ 7

struct ht_node {
  int key;
  int val;
  struct ht_node *next;
};

int hash(int key)
{
  return key * 29;
}

/* returns location of the pointer pointing to node or to the last element of the list */
struct ht_node **find_pnode(struct ht_node **ht, int key, int *found)
{
  struct ht_node **pnode = &ht[hash(key) % HT_SZ];
  while (*pnode != NULL && (*pnode)->key != key)
    pnode = &((*pnode)->next);
  *found = (*pnode != NULL);
  return pnode;
}

void put_ht(struct ht_node **ht, int key, int val)
{
  //  printf("PUT: [%d,%d]\n", key, val);
  int found;
  struct ht_node **pnode = find_pnode(ht, key, &found);
  if (found)
    (*pnode)->val = val;
  else
    {
      struct ht_node *ht_node = malloc(sizeof(struct ht_node));
      ht_node->key = key;
      ht_node->val = val;
      ht_node->next = NULL;
      *pnode = ht_node;
    }
}

int get_ht(struct ht_node **ht, int key)
{
  int found;
  struct ht_node **pnode = find_pnode(ht, key, &found);
  return found ? (*pnode)->val : 0;
}

void free_ht(struct ht_node **ht)
{
  struct ht_node *node0;
  struct ht_node *node1;
  for (int i = 0; i < HT_SZ; i++)
    {
      node0 = ht[i];
      while (node0 != NULL)
	{
	  node1 = node0->next;
	  free(node0);
	  node0 = node1;
	}
    }
}

int main()
{
  struct ht_node *ht[HT_SZ];
  memset(ht, 0, HT_SZ * sizeof(struct ht *));

  int c;
  int l = 0;
  int max_len = 0;
  while((c = getchar()) != EOF)
    {
      if (c == ' ' || c == '\t' || c == '\n')
	{
	  if (l != 0)
	    {
	      put_ht(ht, l, get_ht(ht, l) + 1);
	      if (l > max_len)
		max_len = l;
	      l = 0;
	    }
	}
      else
	{
	  l++;
	}
    }
  printf("max_len=%d\n", max_len);
  for (int i = 1; i <= max_len; i++)
    {
      int count = get_ht(ht, i);
      printf("%3d #", i);
      for(int j = 0; j < count; j++)
	putchar('-');
      printf(">\n");
    }

  free_ht(ht);
}

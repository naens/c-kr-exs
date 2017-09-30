#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SZ 30

struct number_node {
  int number;
  struct number_node *next;
};

void insert_number (struct number_node **ns, int number)
{
  struct number_node *new = malloc(sizeof(struct number_node));
  new->number = number;

  while (*ns && (*ns)->number < number)
    ns = &((*ns)->next);

  new->next = *ns;
  *ns = new;
}

void gen_array(int *a, int len, int min, int max)
{
  struct number_node *numbers = 0;
  for (int i = 0; i < len; i++)
    {
      int n = rand() % (max - min) + min;
      insert_number(&numbers, n);
    }

  struct number_node *prev = numbers;
  for (int i = 0; i < len; i++)
    {
      a[i] = numbers->number;
      numbers = numbers->next;
      free(prev);
      prev = numbers;
    }
}

/* returns first element greater or equal than x or len if no such element */
int bs(int *a, int len, int to_find)
{
  int tree_size = len;
  int from = 0;

  int k;
  if (len == 0 || a[0] > to_find)
    return 0;
  if (a[len - 1] < to_find)
    return len;

  /* now we know that the element is inside the tree! */
  while (tree_size > 1)
    {
      int htsz = tree_size / 2;
      k = from + htsz;
      if (a[k-1] >= to_find)
        tree_size = htsz;
      else
        {
          tree_size -= htsz;
          from = k;
        }
    }
  return from;
}


int main (int argc, char **argv)
{
  srand(time(NULL));
  int x;
  if (argc != 2)
    x = rand() % 0x100;
  else
    x = ((unsigned int)atoi(argv[1])) % SZ;
  printf("x=%02x\n", x);

  int array[SZ];
  gen_array(array, SZ, 0, 0x100);
  for (int i = 0; i < SZ; i++)
    printf("%02x ", array[i]);
  printf("\n");

  /* find element */
  int k = bs(array, SZ, x);

  for (int i = 0; i < k; i++)
    printf("   ");
  printf("^\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SZ 100000
#define MAX 100000
#define N_TESTS 200

/* TEST  RESULTS (200 tests)
 * SZ,MAX  10      100     1000    10000   100000
 * bs1   11.7     25.3     103.8   160.3    282.7
 * bs2   10.6     21.4     33.1     48.6     68.4
 */

double diff_time(struct timespec t0, struct timespec t1)
{
  return (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
}

/* heap sort */
void swp(int *a, int i, int j)
{
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void heapify(int *a, int len, int i)
{
  /* check if leaf */
  if (i >= len / 2)
    return;

  int il = 2 * i + 1;
  int ir = 2 * i + 2;
  int im = ir >= len || a[il] > a[ir] ? il : ir;
  if (a[i] < a[im])
    {
      swp(a, i, im);
      heapify(a, len, im);
    }
}

void heap_insert(int *a, int len, int n)
{
  int i = len;
  a[i] = n;

  int p;
  while (i > 0 && a[i] > a[p=(i-1)/2])
    {
      swp(a, i, p);
      i = p;
    }
}

void heap_sort(int *a, int len)
{
  for (int i = len - 1; i > 0; i--)
    {
      swp(a, 0, i);
      heapify(a, i, 0);
    }
}

void gen_array(int *a, int len, int min, int max)
{
  for (int i = 0; i < len; i++)
    heap_insert(a, i, rand() % (max - min) + min);

  heap_sort(a, len);
  //  for (int i = 0; i < len; i++)
  //    printf("%d ", a[i]);
  //  printf("\n");

  a[len] = -1;                  /* after last element */
}

/* returns first element greater or equal than x or len if no such element */
int bs1(int *a, int len, int to_find)
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

int bs2(int *a, int len, int to_find)
{
  int tree_start = 0;
  int tree_end = len;
  int k;
  while (tree_start < tree_end)
    {
      k = (tree_start + tree_end) / 2;
      if (a[k] < to_find)
        tree_start = k + 1;
      else if (a[k] > to_find)
        tree_end = k;
      else
        return k;
    }
  return tree_end;
}

int array[N_TESTS][SZ + 1];
int x[N_TESTS];

int main (int argc, char **argv)
{
  srand(time(NULL));

  /* generate arrays and random numbers*/
  for (int i = 0; i < N_TESTS; i++)
    {
      gen_array(array[i], SZ, 0, MAX);
      x[i] = rand() % MAX;
    }

  struct timespec t0, t1, t2;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t0);
  for (int i = 0; i < N_TESTS; i++)
    bs1(array[i], SZ, x[i]);

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);

  for (int i = 0; i < N_TESTS; i++)
    bs2(array[i], SZ, x[i]);

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);

  double d1 = (t1.tv_sec - t0.tv_sec) * 1e6 + (t1.tv_nsec - t0.tv_nsec) / 1e3;
  double d2 = (t2.tv_sec - t1.tv_sec) * 1e6 + (t2.tv_nsec - t1.tv_nsec) / 1e3;

  printf("d1=%.1f\nd2=%.1f\n", diff_time(t0, t1), diff_time(t1, t2));
}

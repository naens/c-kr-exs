#include <stdio.h>
#include <stdlib.h>

/* invert n bits from position p
 * 1: n 1, p 0
 * 2: x with n 0 from p]
 * 3: part of x to be inverted
 * 4: invert
 * 5: make result
 */
int invert(unsigned int x, unsigned int p, unsigned int n)
{
  unsigned int r_1 = ~(~0 << n) << p;
  unsigned int r_2 = x & ~r_1;
  unsigned int r_3 = x & r_1;
  unsigned int r_4 = ~r_3 & r_1;
  return r_2 | r_4;
}

int main(int argc, char **argv)
{
  if (argc != 4)
    {
      fprintf(stderr, "bad number of arguments: arguments must be x, p, n\n");
      exit(-99);
    }
  unsigned int x = strtol(argv[1], NULL, 16);
  unsigned int p = strtol(argv[2], NULL, 16);
  unsigned int n = strtol(argv[3], NULL, 16);
  printf("invert(%x, %x, %x) = %x\n", x, p, n, invert(x, p, n));
}

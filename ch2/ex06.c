#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>


/* x ABCDEFGH
 * p 76543210
 * y abcdefgh
 * n    <-n-> 
 * r AfghEFGH p=4,n=3
 *
 * 1: get x without n bits starting from p:
 *   A000EFGH
 * 2: get n bits from y:
 *   00000fgh
 * 3: shift bits from y:
 *   0fgh0000
 * 4: construct the result from 1 and 3
 *   AfghEFGH
 */
unsigned int setbits(unsigned int x, int p, int n, unsigned int y)
{
  unsigned int nbits = ~(~0 << n);
  unsigned int r_1 = ~(nbits << p) & x;
  unsigned int r_2 = (y & nbits) << p;
  return r_1 | r_2;
}

int main()
{
  char *str;
  unsigned int x;
  int p;
  int n;
  unsigned int y;
  /* arguments: x(%x) p(%d) n(%d) y(%x) */
  while (str = readline("[]"))
    {
      char *tmp;
      tmp = strtok(str, " ");
      if (!tmp)
        break;
      x = strtol(tmp, NULL, 16);
      tmp = strtok(NULL, " ");
      if (!tmp)
        break;
      p = atoi(tmp);
      tmp = strtok(NULL, " ");
      if (!tmp)
        break;
      n = atoi(tmp);
      tmp = strtok(NULL, " ");
      if (!tmp)
        break;
      y = strtol(tmp, NULL, 16);
      printf("setbits(x=%x, p=%d, n=%d, y=%x) = %x\n", x, p, n, y, setbits(x, p, n, y));
    }
}
/* EXAMPLES
 * x 01010101 (x=0x55)
 * p=3, n=3
 * y 0011 0101 (y=0x35)
 *   0000 0101 (5)
 *   0010 1000 (0x28)
 * r 0110 1101 (0x6d)
 *
 * x 0010 0100 1001 0010 (x=0x2492)
 * p=4, n=5
 * y 0100 1111 0101 0101 (y=0x4f55)
 *   0000 0000 0001 0101 (0x15)
 *   0000 0001 0101 0000 (0x150)
 * r 0010 0101 0101 0010 (0x2552)
 */

/* x = x & (x - 1)
 * if last bit is 1
 * x    XXXXXXX1
 * x-1  XXXXXXX0
 * &    XXXXXXX0
 *
 * if x has 0's
 * x    XXXX1000
 * x-1  XXXX0111
 * &    XXXX0000
 *
 * -> because substracting 1 changes all bits including the last bit
 */

#include <stdio.h>
#include <stdlib.h>

int bitcount2(unsigned int x)
{
  unsigned int tmp = x;
  int count = 0;
  while (tmp)
    {
      count++;
      tmp&= (tmp - 1);
    }

  return count;
}

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "number of args must be 1, arg must be in hex\n");
      exit(9087);
    }

  unsigned int number = strtol(argv[1], NULL, 16);
  printf("%x\n", bitcount2(number));
}

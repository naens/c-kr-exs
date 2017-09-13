#include <stdio.h>
#include <stdlib.h>

unsigned int rotate(unsigned int x, unsigned int n)
{
  int len = sizeof (unsigned int) * 8;
  int firstbits = ~(~0 << n) & x;
  int lshift = len - n;
  return (firstbits << lshift) | (x >> n);
}


int main(int argc, char **argv)
{
  if (argc != 3)
    {
      fprintf(stderr, "arguments: x, n in hex\n");
      exit(-21432);
    }
  unsigned int x = strtol(argv[1], NULL, 16);
  unsigned int n = strtol(argv[2], NULL, 16);
  printf("%x\n", rotate(x, n));
}

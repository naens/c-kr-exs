#include <stdio.h>

int main()
{
  printf("EOF=%d\n", EOF);
  printf("EOF=%llx\n", (unsigned long long int)EOF);
  char c = -1;
  printf("%d\n", c);
}

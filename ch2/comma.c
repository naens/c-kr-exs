#include <stdio.h>

int main()
{
  int a;
  int b;
  int c = (a = 1, b = 10);
  printf("%d\n", c);
}

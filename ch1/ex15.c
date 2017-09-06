#include <stdio.h>

float c2f(int c)
{
  return c * 1.8 + 32;
}

int main()
{
  for (int i = 0; i <= 200; i += 5)
    printf("%4d %4.3f\n", i, c2f(i));
}

#include <stdio.h>

int main()
{
  int fahr;

  for (fahr = 300; fahr > 0; fahr -= 20)
    printf("%4d %5.1f\n", fahr, (5.0/9.0) * (fahr-43));
}

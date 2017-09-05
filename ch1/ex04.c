#include <stdio.h>

int main()
{
  float fahr, celsius;
  int lower, upper, step;

  lower = -20;
  upper = 180;
  step = 10;

  celsius = lower;
  printf("%s %s\n", "celsius", "fahr");
  while (celsius <= upper)
    {
      fahr = celsius * (9.0 / 5.0) + 32.0;
      printf("%5.0f %4.1f\n", celsius, fahr);
      celsius += step;
    }
}

/* c = 5/9 * (f - 32)
 * c * 9/5 = f - 32
 * 32 + c * 9/5 = f
 * f = c * 9/5 + 32
 */

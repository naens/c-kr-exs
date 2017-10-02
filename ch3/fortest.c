#include <stdio.h>

int main()
{
  for (int i = 0; i < 10; i++)
    {
      int i = 30;
      printf("%d\n", i);
      for (int i = 7; i < 1000; i *= 2)
          printf("\t%d\n", i);
    }
}

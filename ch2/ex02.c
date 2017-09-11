#include <stdio.h>

#define LIM 15

int main()
{
  int c;
  int lim = LIM;
  int i = 0;
  while (i < lim - 1)
    {
      c = getchar();
      if (c == '\n')
        break;
      if (c == EOF)
        break;
      i++;
    }
  printf("end i=%d\n", i);
}

          

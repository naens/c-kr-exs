#include <stdio.h>

int main()
{
  int x;
  do {
    x = (getchar() != EOF);
    printf("x=%d\n", x);
  } while (x);
}

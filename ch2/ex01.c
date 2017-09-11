#include <stdio.h>
#include <limits.h>

int main()
{
  printf("max signed char=%d\nmin signed char=%d\n", CHAR_MAX, CHAR_MIN);
  printf("max unsigned char=%d\nmin unsigned char=%d\n\n", UCHAR_MAX, 0);

  printf("max signed short=%d\nmin signed short=%d\n", SHRT_MAX, SHRT_MIN);
  printf("max unsigned short=%d\nmin unsigned short=%d\n\n", USHRT_MAX, 0);

  printf("max signed int=%d\nmin signed int=%d\n", INT_MAX, INT_MIN);
  printf("max unsigned int=%d\nmin unsigned int=%d\n\n", UINT_MAX, 0);

  printf("max signed long=%ld\nmin signed long=%ld\n", LONG_MAX, LONG_MIN);
  printf("max unsigned long=%llu\nmin unsigned long=%lld\n", ULONG_MAX, 0);

  float f1 = 1;
  float f2 = 10000;
  while (f2 > f1)
    {
      f1 *= 10;
      f2 *= 10;
    }
  printf("%f\n", f1);
}

#include <stdio.h>

#define swap(t,x,y) {t z=(x);(x)=(y);(y)=z;}

int main(int argc, char **argv)
{
  int a = 1;
  int b = 2;
  printf("a=%d, b=%d\n", a, b);
  swap(int,a,b);
  printf("a=%d, b=%d\n", a, b);

  char c1 = 'a';
  char c2 = 'b';
  printf("c1=%c, c2=%c\n", c1, c2);
  swap(char,c1,c2);
  printf("c1=%c, c2=%c\n", c1, c2);

  char *s1 = "abcd";
  char *s2 = "0123";
  printf("s1=%s, s2=%s\n", s1, s2);
  swap(char*,s1,s2);
  printf("s1=%s, s2=%s\n", s1, s2);
}

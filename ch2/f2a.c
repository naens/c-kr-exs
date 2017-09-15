#include <stdio.h>

void dothings()
{
  printf("doing things\n");
}

int main()
{
  unsigned char *pch;
  pch = (void (*))dothings;
  for (int i = 0; i < 20; i++)
    printf("%02x ", pch[i]);
  printf("\n");i
}

/* 
objdump -d f2a
00000000000006da <dothings>:
 6da:	55                   	push   %rbp
 6db:	48 89 e5             	mov    %rsp,%rbp
 6de:	48 8d 3d df 00 00 00 	lea    0xdf(%rip),%rdi        # 7c4 <_IO_stdin_used+0x4>
 6e5:	e8 b6 fe ff ff       	callq  5a0 <puts@plt>
 6ea:	90                   	nop
 6eb:	5d                   	pop    %rbp
 6ec:	c3                   	retq 

output:
55 48 89 e5 48 8d 3d ef 00 00 00 e8 b6 fe ff ff 90 5d c3 55 
 */

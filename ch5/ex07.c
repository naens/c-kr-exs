#include <stdio.h>

#define MAX_HEIGHT 4
#define MAX_WIDTH 8

int readlines(char *lptr, int max_length, int max_hight)
{
    int c;
    int x = 0;
    int y = 0;
    while (y < max_hight && (c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            *(lptr + y * max_length + x) = 0; /* terminate previous line */
            y++;
            if (x == 0)         /* empty line terminates input */
              break;
            x = 0;
        }
        else if (x < max_length - 1)
        {
            *(lptr + y * max_length + x) = c;
            x++;
        }
    }
    return y;
}

int main(int argc, char **argv)
{
    char buf[MAX_HEIGHT][MAX_WIDTH];
    int lines = readlines((char *)buf, MAX_WIDTH, MAX_HEIGHT);
    printf("lines = %d\n", lines);
    for (int i = 0; i < lines ; i++)
      printf("[%d] '%s'\n", i, buf[i]);
}

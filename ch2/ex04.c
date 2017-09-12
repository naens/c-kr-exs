#include <stdio.h>
#include <readline/readline.h>

/* check if str contains c */
int contains(char *str, char c)
{
  while (*str)
    if (*(str++) == c)
      return 1;
  return 0;
}

void squeeze(char *str, char *chars)
{
  int i = 0;
  int j = 0;

  while (str[i])
    {
      if (!contains(chars, str[i]))
        str[j++] = str[i];
      i++;
    }
  str[j] = 0;
}

int main()
{
  char *str;
  char *str2;
  while (str = readline("[]"))
    {
      if (strlen(str) == 0)
        break;

      /* get 2 arguments */
      str = strtok(str, " ");
      str2 = strtok(NULL, " ");
      printf("arg1=\"%s\" arg2=\"\%s\"\n", str, str2);
      if (str && str2)
        {
          squeeze(str, str2);
          printf("%s\n", str);
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

void fsize(char *name, int level)
{
    struct dirent *dir;
    DIR *d;
    printf("%s:\n", name);
    d = opendir(name);    
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)
        {
            struct stat st;
            char fn[0x100];
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            for (int i = 0; i < level; i++)
                printf("   ");
            sprintf(fn, "%s/%s", name, dir->d_name);
            if (stat(fn, &st) == -1)
                exit(1);
            
            if (S_ISDIR(st.st_mode))
                fsize(fn, level + 1);
            else
                printf("%s(%d): %d\n", dir->d_name, st.st_size, st.st_ino);
        }
        closedir(d);
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".", 1);
    else for(int i = 1; i < argc; i++)
        fsize(argv[i], 1);
    exit(0);
}

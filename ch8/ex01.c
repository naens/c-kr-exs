#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd;
    int c;

    if (argc == 1) {
        while (read(0, &c, 1) == 1)
            write(1, &c, 1);
    } else {
        for (int i = 1; i < argc; i++) {
            if ((fd = open(argv[i], O_RDONLY, 0)) == -1)
                return 1;
            while (read(fd, &c, 1) == 1)
                write(1, &c, 1);
            close(fd);
        }
    }
    return 0;
}

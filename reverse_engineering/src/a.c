#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    const char *filename = "./pesho";
    char buffer[1024];

    int fd = open(filename, O_RDONLY);
    /**
     * Missing check for a valid fd
     * The './pesho' file is missing on the file system or the relative path is
     * incorrect.
     */

    ssize_t n = read(fd, buffer, sizeof buffer);
    if (n == -1) {
        return -1;
    }
    close(fd);

    return 0;
}

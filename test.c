#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    const char *filename = "test.txt";

    int fd = open(filename, O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // -------------------------
    // Prepare iovec for writev2
    // -------------------------
    const char *part1 = "Hello ";
    const char *part2 = "from ";
    const char *part3 = "pwritev2!\n";

    struct iovec iovw[3];
    iovw[0].iov_base = (void *)part1;
    iovw[0].iov_len  = strlen(part1);

    iovw[1].iov_base = (void *)part2;
    iovw[1].iov_len  = strlen(part2);

    iovw[2].iov_base = (void *)part3;
    iovw[2].iov_len  = strlen(part3);

    // -------------------------
    // pwritev2(fd, iov, 3, offset=0, flags=0)
    // -------------------------
    ssize_t written = pwritev2(fd, iovw, 3, 0, 0);
    if (written < 0) {
        perror("pwritev2");
        close(fd);
        return 1;
    }

    // -------------------------
    // Prepare buffers for preadv2
    // -------------------------
    char buf1[16];
    char buf2[16];

    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));

    struct iovec iovr[2];
    iovr[0].iov_base = buf1;
    iovr[0].iov_len  = sizeof(buf1) - 1;

    iovr[1].iov_base = buf2;
    iovr[1].iov_len  = sizeof(buf2) - 1;

    // -------------------------
    // preadv2(fd, iov, 2, offset=0, flags=0)
    // -------------------------
    ssize_t readn = preadv2(fd, iovr, 2, 0, 0);
    if (readn < 0) {
        perror("preadv2");
        close(fd);
        return 1;
    }

    printf("buf1: %s\n", buf1);
    printf("buf2: %s\n", buf2);

    close(fd);
    return 0;
}
/**
 * parser/syscalls/fs.c - Implementation of filesystem syscall parsers.
 */

#include <stdio.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/helpers.h"
#include "parser/syscalls/fs.h"

const struct parser_struct fs_syscalls[] = {
#define X(name, nr) { nr, SYSCALL_PARSER_NAME(name) },
FS_SYSCALL_LIST
#undef X
};

const char *STDFD_NAMES[] = {
        [0] = "STDIN",
        [1] = "STDOUT",
        [2] = "STDERR",
};

/* read(int fd, void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(read)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
};

/* write(int fd, const void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(write)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
};

/* open(const char *path, int flags) */
DEFINE_SYSCALL_PARSER(open)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);

        return 0;
}

/* close(int fd) */
DEFINE_SYSCALL_PARSER(close)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* rename(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_PARSER(rename)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* unlink(const char *path) */
DEFINE_SYSCALL_PARSER(unlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* link(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_PARSER(link)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* symlink(const char *target, const char *linkpath) */
DEFINE_SYSCALL_PARSER(symlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* lseek(int fd, off_t offset, int whence) */
DEFINE_SYSCALL_PARSER(lseek)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* pread64(int fd, void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_PARSER(pread64)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[3]);

        return 0;
}

/* pwrite64(int fd, const void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_PARSER(pwrite64)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[3]);

        return 0;
}

/* readlink(const char *path, char *buf, size_t count) */
DEFINE_SYSCALL_PARSER(readlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* getcwd(char *buf, size_t size) */
DEFINE_SYSCALL_PARSER(getcwd)
{
        FMT_ADDR(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);

        return 0;
}

/* chdir(const char *path) */
DEFINE_SYSCALL_PARSER(chdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}
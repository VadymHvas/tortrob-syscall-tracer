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
        int n;

        TRY_FMT_1(fmt_fd, ctx, args[0], &n);
        TRY_FMT_WITH_SEP_1(fmt_addr, ctx, args[1], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[2], &n);

        return 0;
};

/* write(int fd, const void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(write)
{
        int n;

        TRY_FMT_1(fmt_fd, ctx, args[0], &n);
        TRY_FMT_WITH_SEP_2(fmt_string_from_mem, ctx, args[1], args[2], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[2], &n);

        return 0;
};

/* open(const char *path, int flags) */
DEFINE_SYSCALL_PARSER(open)
{
        int n;
       
        TRY_FMT_2(fmt_string_from_mem, ctx, args[0], args[1], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[1], &n);

        return 0;
}

/* close(int fd) */
DEFINE_SYSCALL_PARSER(close)
{
        int n;

        TRY_FMT_1(fmt_fd, ctx, args[0], &n);

        return 0;
}

/* rename(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_PARSER(rename)
{
        int n;

        TRY_FMT_2(fmt_string_from_mem, ctx, args[0], NAME_MAX, &n);
        TRY_FMT_WITH_SEP_2(fmt_string_from_mem, ctx, args[1], NAME_MAX, &n);

        return 0;
}

/* unlink(const char *path) */
DEFINE_SYSCALL_PARSER(unlink)
{
        int n;

        TRY_FMT_2(fmt_string_from_mem, ctx, args[0], NAME_MAX, &n);

        return 0;
}

/* link(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_PARSER(link)
{
        int n;

        TRY_FMT_2(fmt_string_from_mem, ctx, args[0], NAME_MAX, &n);
        TRY_FMT_WITH_SEP_2(fmt_string_from_mem, ctx, args[1], NAME_MAX, &n);

        return 0;
}

/* symlink(const char *target, const char *linkpath) */
DEFINE_SYSCALL_PARSER(symlink)
{
        int n;

        TRY_FMT_2(fmt_string_from_mem, ctx, args[0], NAME_MAX, &n);
        TRY_FMT_WITH_SEP_2(fmt_string_from_mem, ctx, args[1], NAME_MAX, &n);

        return 0;
}

/* lseek(int fd, off_t offset, int whence) */
DEFINE_SYSCALL_PARSER(lseek)
{
        int n;

        TRY_FMT_1(fmt_fd, ctx, args[0], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[1], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[2], &n);

        return 0;
}

/* pread64(int fd, void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_PARSER(pread64)
{
        int n;

        TRY_FMT_1(fmt_fd, ctx, args[0], &n);
        TRY_FMT_WITH_SEP_1(fmt_addr, ctx, args[1], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[2], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[3], &n);

        return 0;
}

/* pwrite64(int fd, const void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_PARSER(pwrite64)
{
        int n;

        TRY_FMT_1(fmt_fd, ctx, args[0], &n);
        TRY_FMT_WITH_SEP_2(fmt_string_from_mem, ctx, args[1], args[2], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[2], &n);
        TRY_FMT_WITH_SEP_1(fmt_int, ctx, args[3], &n);

        return 0;
}
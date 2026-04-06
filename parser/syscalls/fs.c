/**
 * parser/syscalls/fs.c - Implementation of filesystem syscall parsers.
 */

#include <stdio.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/helpers.h"

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

        return fmt_fd(ctx, args[0], &n);
}
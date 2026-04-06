/**
 * parser/syscalls/fs.c - Implementation of filesystem syscall parsers.
 */

#include <stdio.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/helpers.h"

static const char *STDFD_NAMES[] = {
        [0] = "STDIN",
        [1] = "STDOUT",
        [2] = "STDERR",
};

/* read(int fd, void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(read)
{
        int n;

        if (args[0] > 2) {
                if (fmt_int(ctx, args[0], &n))
                        return 1;
        } else {
                if (fmt_string(ctx, STDFD_NAMES[args[0]], &n))
                        return 1;
        }

        if (fmt_separator(ctx, &n))
                return 1;

        if (fmt_addr(ctx, args[1], &n))
                return 1;
        
        if (fmt_separator(ctx, &n))
                return 1;

        if (fmt_int(ctx, args[2], &n))
                return 1;

        return 0;
};

/* write(int fd, const void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(write)
{
        int n;

        if (args[0] > 2) {
                if (fmt_int(ctx, args[0], &n))
                        return 1;
        } else {
                if (fmt_string(ctx, STDFD_NAMES[args[0]], &n))
                        return 1;
        }

        if (fmt_separator(ctx, &n))
                return 1;

        if (fmt_addr(ctx, args[1], &n))
                return 1;
        
        if (fmt_separator(ctx, &n))
                return 1;

        if (fmt_int(ctx, args[2], &n))
                return 1;

        return 0;
};

/* open(const char *path, int flags) */
DEFINE_SYSCALL_PARSER(open)
{
        int n;
       
        if (fmt_addr(ctx, args[0], &n))
                return 1;

        if (fmt_separator(ctx, &n))
                return 1;

        if (fmt_int(ctx, args[1], &n))
                return 1;

        return 0;
}

/* close(int fd) */
DEFINE_SYSCALL_PARSER(close)
{
        int n;

        if (args[0] > 2)
                return fmt_int(ctx, args[0], &n);
        else
                return fmt_string(ctx, STDFD_NAMES[args[0]], &n);
}
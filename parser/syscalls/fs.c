/**
 * parser/syscalls/fs.c - Implementation of filesystem syscall parsers.
 */

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/parser.h"

DEFINE_SYSCALL_PARSER(read)
{
        int n;

        /* TODO: Refactor this stuff. */
        n = snprintf(buf + *offset, bufsize - *offset, "%d", (int)args[0]);
        if (n < 0) return 1;

        *offset += n;
        if (*offset >= bufsize) return 1;

        n = snprintf(buf + *offset, bufsize - *offset, ", 0x%llx",
                        (unsigned long long)args[1]);
        if (n < 0) return 1;

        *offset += n;
        if (*offset >= bufsize) return 1;

        n = snprintf(buf + *offset, bufsize - *offset, ", %llu",
                        (unsigned long long)args[2]);
        if (n < 0) return 1;

        *offset += n;
        if (*offset >= bufsize) return 1;

        return 0;
};
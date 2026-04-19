/*
 * args/proc.c - Implementation of process-related syscall parsers.
 */

#include "parser/args.h"
#include "args/helpers.h"

#include "args/proc.h"

#include "args/proc/flags.h"
#include "args/proc/struct.h"

const struct parser_struct proc_syscalls[] = {
#define X(name, nr, has_enter, has_exit) \
        DEFINE_PARSER( \
                nr, \
                ENTER_IF_##has_enter(name), \
                EXIT_IF_##has_exit(name) \
        ), \

PROC_SYSCALL_LIST
#undef X
};

/* exit(int status) */
DEFINE_SYSCALL_ENTER_PARSER(exit)
{
        FMT_INT(ctx, args[0]);

        return 0;
}

/* exit_group(int status) */
DEFINE_SYSCALL_ENTER_PARSER(exit_group)
{
        FMT_INT(ctx, args[0]);

        return 0;
}

/* wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage) */
DEFINE_SYSCALL_ENTER_PARSER(wait4)
{
        FMT_INT(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(wait4)
{
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_WAIT4_FLAGS(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_RUSAGE_STRUCT(ctx, args[3]);

        return 0;
}
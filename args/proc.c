/*
 * args/proc.c - Implementation of process-related syscall parsers.
 */

#include "parser/args.h"
#include "args/helpers.h"
#include "args/array.h"
#include "args/fs/flags.h"

#include "args/proc.h"

#include "args/proc/flags.h"
#include "args/proc/struct.h"
#include "args/proc/value.h"

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
        FMT_WAIT4_STATUS(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_WAIT4_FLAGS(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_RUSAGE_STRUCT(ctx, args[3]);

        return 0;
}

/* waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options) */
DEFINE_SYSCALL_ENTER_PARSER(waitid)
{
        FMT_IDTYPE(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(waitid)
{
        FMT_SIGINFO_STRUCT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_WAIT4_FLAGS(ctx, args[3]);

        return 0;
}

/* clone(unsigned long flags,
           void *child_stack,
           int *ptid,
           int *ctid,
           unsigned long newtls) */
DEFINE_SYSCALL_ENTER_PARSER(clone)
{
        FMT_STRING(ctx, "flags=");
        FMT_CLONE_FLAGS(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING(ctx, "child_stack=");
        FMT_ADDR(ctx, args[1]);

        return 0;
}

/* clone3(struct clone_args *cl_args, size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(clone3)
{
        FMT_CLONE_ARGS_STRUCT(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[1]);

        return 0;
}

/* execve(const char *pathname, char *const argv[], char *const envp[]) */
DEFINE_SYSCALL_ENTER_PARSER(execve)
{
        FMT_CSTRING_MEM(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_ARR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_ARR(ctx, args[2]);

        return 0;
}

/* execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags) */
DEFINE_SYSCALL_ENTER_PARSER(execveat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_CSTRING_MEM(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_ARR(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_ARR(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[4]);

        return 0;
}
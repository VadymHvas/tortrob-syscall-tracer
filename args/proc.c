/*
 * args/proc.c - Implementation of process-related syscall parsers.
 */

#include <signal.h>

#include "parser/args.h"
#include "args/helpers.h"
#include "args/array.h"
#include "args/fs/flags.h"
#include "args/fs/struct.h"

#include "args/proc.h"

#include "args/proc/flags.h"
#include "args/proc/struct.h"
#include "args/proc/value.h"
#include "args/proc/mask.h"

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

/* kill(pid_t pid, int sig) */
DEFINE_SYSCALL_ENTER_PARSER(kill)
{
        FMT_INT(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_SIGNAL_NAME(ctx, args[1]);

        return 0;
}

/* tkill(pid_t pid, int sig) [DEPRECATED] */
DEFINE_SYSCALL_ENTER_PARSER(tkill)
{
        FMT_INT(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_SIGNAL_NAME(ctx, args[1]);

        return 0;
}

/* tgkill(pid_t tgid, pid_t tid, int sig) */
DEFINE_SYSCALL_ENTER_PARSER(tgkill)
{
        FMT_INT(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_SIGNAL_NAME(ctx, args[2]);

        return 0;
}

/* rt_sigaction(int sig, const struct sigaction *act, struct sigaction *oldact) */
DEFINE_SYSCALL_ENTER_PARSER(rt_sigaction)
{
        FMT_SIGNAL_NAME(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_SIGACTION_STRUCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(rt_sigaction)
{
        FMT_STRUCT_IF_OK(FMT_SIGACTION_STRUCT, ctx, args[2], ctx->retval);
        
        return 0;
}

/* rt_sigprocmask(int how, sigset_t *nset, sigset_t *oset, size_t sigsetsize) */
DEFINE_SYSCALL_ENTER_PARSER(rt_sigprocmask)
{
        FMT_SIGPROCMASK_HOW(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_SIGSET(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_SIGSET(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);

        return 0;
}

/* rt_sigpending(sigset_t *uset, size_t sigsetsize) */
DEFINE_SYSCALL_ENTER_PARSER(rt_sigpending)
{
        FMT_SIGSET(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[1]);

        return 0;
}

/* rt_sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout, size_t sigsetsize) */
DEFINE_SYSCALL_ENTER_PARSER(rt_sigtimedwait)
{
        FMT_SIGSET(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(rt_sigtimedwait)
{
        FMT_STRUCT_IF_OK(FMT_SIGINFO_STRUCT, ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_STRUCT_IF_OK(FMT_TIMESPEC_STRUCT, ctx, args[2], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);

        return 0;
}

/* rt_sigqueueinfo(pid_t pid, int sig, const siginfo_t *info) */
DEFINE_SYSCALL_ENTER_PARSER(rt_sigqueueinfo)
{
        FMT_INT(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_SIGNAL_NAME(ctx, args[1]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(rt_sigqueueinfo)
{
        FMT_STRUCT_IF_OK(FMT_SIGINFO_STRUCT, ctx, args[2], ctx->retval);

        return 0;
}

/* rt_sigsuspend(const sigset_t *sigset, size_t sigsetsize) */
DEFINE_SYSCALL_ENTER_PARSER(rt_sigsuspend)
{
        FMT_SIGSET(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[1]);

        return 0;
}
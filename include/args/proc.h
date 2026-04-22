#pragma once

#include "parser/args.h"

#define PROC_SYSCALL_LIST \
        X(exit,            60,  1, 0) \
        X(exit_group,      231, 1, 0) \
        X(wait4,           61,  1, 1) \
        X(waitid,          247, 1, 1) \
        X(clone,           56,  1, 0) \
        X(clone3,          435, 1, 0) \
        X(execve,          59,  1, 0) \
        X(execveat,        322, 1, 0) \
        X(kill,            62,  1, 0) \
        X(tkill,           200, 1, 0) \
        X(tgkill,          234, 1, 0) \
        X(rt_sigaction,    13,  1, 1) \
        X(rt_sigpending,   127, 1, 0) \
        X(rt_sigtimedwait, 128, 1, 1) \
        X(rt_sigqueueinfo, 129, 1, 1) \
        X(rt_sigsuspend,   130, 1, 0)
        
#define PROC_SYSCALL_COUNT 16

#define X(name, nr, has_enter, has_exit) \
        DEFINE_SYSCALL_ENTER_PARSER(name);
PROC_SYSCALL_LIST
#undef X

#define X(name, nr, has_enter, has_exit) \
        DEFINE_SYSCALL_EXIT_PARSER(name);
PROC_SYSCALL_LIST
#undef X

extern const struct parser_struct proc_syscalls[PROC_SYSCALL_COUNT];
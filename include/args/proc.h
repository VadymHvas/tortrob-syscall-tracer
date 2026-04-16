#pragma once

#include "parser/args.h"

#define PROC_SYSCALL_LIST \
        X(exit,       60,  1, 0) \
        X(exit_group, 231, 1, 0)

#define PROC_SYSCALL_COUNT 2

#define X(name, nr, has_enter, has_exit) \
        DEFINE_SYSCALL_ENTER_PARSER(name);
PROC_SYSCALL_LIST
#undef X

#define X(name, nr, has_enter, has_exit) \
        DEFINE_SYSCALL_EXIT_PARSER(name);
PROC_SYSCALL_LIST
#undef X

extern const struct parser_struct proc_syscalls[PROC_SYSCALL_COUNT];
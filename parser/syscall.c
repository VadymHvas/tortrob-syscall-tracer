/*
 * parser/syscall.c - Syscall parser.
 *
 * This module provides methods to syscall parsing by it's number.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#include "parser/syscall.h"
#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/helpers.h"

static const struct syscall_entry *get_syscall_by_nr(long nr);

int get_syscall_with_args(struct user_regs_struct *regs, char *buf, size_t bufsize)
{
        const struct syscall_entry *syscall = get_syscall_by_nr(regs->orig_rax);
        raw_reg args[6];
        
        if (!syscall)
                return -1;
        
        abi_get_syscall_args(regs, args);
        fmt_syscall(buf, bufsize, syscall, args);

        return 0;
}

static const struct syscall_entry *get_syscall_by_nr(long nr)
{
        if (nr >= 0 && nr < SYSCALL_COUNT)
                return &syscall_table[nr];

        return NULL;
}
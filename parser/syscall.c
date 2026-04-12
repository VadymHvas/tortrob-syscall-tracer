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
#include "parser/syscalls/args/helpers.h"

static const struct syscall_entry *get_syscall_by_nr(long nr);

int begin_syscall_fmt(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        reg_t args[6];
        const struct syscall_entry *syscall = get_syscall_by_nr(regs->orig_rax);

        if (!syscall)
                return -1;

        abi_get_syscall_args(regs, args);
        fmt_syscall_enter(ctx, syscall, args);

        return 0;
}

int finalize_syscall_fmt(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        reg_t args[6];
        const struct syscall_entry *syscall = get_syscall_by_nr(regs->orig_rax);

        if (!syscall)
                return -1;

        abi_get_syscall_args(regs, args);
        
        if (syscall->args)
                syscall_parse(ctx, syscall, args);

        FMT_STRING(ctx, ")");
        FMT_STRING(ctx, " = ");
        FMT_LLU(ctx, ctx->retval);

        return 0;
}

static const struct syscall_entry *get_syscall_by_nr(long nr)
{
        if (nr >= 0 && nr < SYSCALL_COUNT)
                return &syscall_table[nr];

        return NULL;
}
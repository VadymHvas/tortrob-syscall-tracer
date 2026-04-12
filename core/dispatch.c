/*
 * core/dispatch.c - Syscall entry/exit handler and state manager.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#include "core/dispatch.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscall.h"
#include "parser/syscalls/args/helpers.h"

static void enter_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        begin_syscall_fmt(ctx, regs);
        
        ctx->in_syscall = SYSCALL_EXIT;
}

static void exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        ctx->retval = abi_get_retval(regs);

        finalize_syscall_fmt(ctx, regs);
        printf("%s\n", ctx->buf);

        ctx->in_syscall = SYSCALL_ENTER;
        ctx->offset = 0;
}

void enter_or_exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        if (!ctx->in_syscall) {
                enter_syscall(ctx, regs);
        } else {
                exit_syscall(ctx, regs);
        }
}
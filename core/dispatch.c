/*
 * core/dispatch.c - Syscall entry/exit handler and state manager.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#include "core/dispatch.h"
#include "parser/syscall.h"
#include "parser/syscalls/args/helpers.h"

static void entry_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        get_syscall_with_args(ctx, regs);
        
        if (!ctx->delayed)
                printf("%s", ctx->buf);
        
        ctx->in_syscall = SYSCALL_EXIT;
}

// TODO: Make error handling inside deferred system call parsers.
static void exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        if (ctx->delayed) {
                get_syscall_with_args(ctx, regs);
                printf("%s = %llu\n", ctx->buf, regs->rax);
        } else {
                printf(" = %llu\n", regs->rax);
        }

        CLEANUP_PARSER_CTX(ctx);
}

void entry_or_exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        if (!ctx->in_syscall) {
                entry_syscall(ctx, regs);
        } else {
                exit_syscall(ctx, regs);
        }
}
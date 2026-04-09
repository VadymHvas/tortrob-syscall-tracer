/*
 * core/dispatch.c - Syscall entry/exit handler and state manager.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#include "core/dispatch.h"
#include "parser/syscall.h"
#include "parser/syscalls/args/helpers.h"

/* Too basic logic.
 *
 * TODO:
 *
 * Implement a more advanced algorithm
 * for generating the output string of a
 * syscall with arguments 
 * 
 */
static void entry_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        get_syscall_with_args(ctx, regs);
        printf("%s", ctx->buf);
}

static void exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        printf("(ret=%llu)\n", regs->rax);
        CLEANUP_PARSER_CTX(ctx);
}

void entry_or_exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs)
{
        if (!ctx->in_syscall) {
                entry_syscall(ctx, regs);
                ctx->in_syscall = SYSCALL_EXIT;
        } else {
                exit_syscall(ctx, regs);
        }
}
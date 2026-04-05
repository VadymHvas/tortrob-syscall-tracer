/*
 * core/dispatch.c - Syscall entry/exit handler and state manager.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#include "core/dispatch.h"
#include "parser/syscall.h"

#define __SYSCALL_BUF_SIZE 128

#define __SWAP_SYSCALL_STATE(s) (s = s ? SYSCALL_ENTRY : SYSCALL_EXIT)

typedef enum {
        SYSCALL_ENTRY = 0,      /* Before entering the syscall. */
        SYSCALL_EXIT  = 1       /* Before exiting the syscall. */
} __syscall_state_t;

static __syscall_state_t in_syscall = SYSCALL_ENTRY;

/* Too basic logic.
 *
 * TODO:
 *
 * Implement a more advanced algorithm
 * for generating the output string of a
 * syscall with arguments 
 * 
 */
static void __entry_syscall(struct user_regs_struct *regs)
{
        char buf[__SYSCALL_BUF_SIZE];
        get_syscall_with_args(regs, buf, __SYSCALL_BUF_SIZE);
        printf("%s", buf);
}

static void __exit_syscall(struct user_regs_struct *regs)
{
        printf("(ret=%llu)\n", regs->rax);
}

void entry_or_exit_syscall(struct user_regs_struct *regs)
{
        if (!in_syscall) {
                __entry_syscall(regs);
        } else {
                __exit_syscall(regs);
        }

        __SWAP_SYSCALL_STATE(in_syscall);
}
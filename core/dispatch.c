/*
 * core/dispatch.c - Syscall entry/exit handler and state manager.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/user.h>

#include "core/dispatch.h"
#include "parser/syscall.h"

#define SYSCALL_BUF_SIZE 128

#define SWAP_SYSCALL_STATE(s) (s = s ? SYSCALL_ENTRY : SYSCALL_EXIT)

typedef enum {
        SYSCALL_ENTRY = 0,      /* Before entering the syscall. */
        SYSCALL_EXIT  = 1       /* Before exiting the syscall. */
} syscall_state_t;

static syscall_state_t in_syscall = SYSCALL_ENTRY;

/* Too basic logic.
 *
 * TODO:
 *
 * Implement a more advanced algorithm
 * for generating the output string of a
 * syscall with arguments 
 * 
 */
static void entry_syscall(struct user_regs_struct *regs, pid_t tracee)
{
        char buf[SYSCALL_BUF_SIZE];
        get_syscall_with_args(regs, buf, SYSCALL_BUF_SIZE, tracee);
        printf("%s", buf);
}

static void exit_syscall(struct user_regs_struct *regs, pid_t tracee)
{
        printf("(ret=%llu)\n", regs->rax);
}

void entry_or_exit_syscall(struct user_regs_struct *regs, pid_t tracee)
{
        if (!in_syscall) {
                entry_syscall(regs, tracee);
        } else {
                exit_syscall(regs, tracee);
        }

        SWAP_SYSCALL_STATE(in_syscall);
}
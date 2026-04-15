#pragma once

#include <sys/user.h>
#include <sys/types.h>

#include "args/helpers.h"

/**
 * syscall_state_t - Enum to track syscall entry and exit states.
 */
typedef enum {
        SYSCALL_ENTER = 0,      /* Before entering the syscall. */
        SYSCALL_EXIT  = 1       /* Before exiting the syscall. */
} syscall_state_t;

/**
 * enter_or_exit_syscall - High-level dispatcher for intercepted system calls.
 * @ctx:    Parser context.
 * @regs:   Pointer to the current CPU register state of the tracee.
 * 
 * This function acts as the primary logic gate for the tracer. It tracks 
 * whether the process is entering or exiting a system call by maintaining 
 * an internal state.
 * - On Enter: Extracts and prints syscall name and arguments.
 * - On Exit:  Extracts and prints the return value from the RAX register.
 *
 * Note: This function assumes that PTRACE_O_TRACESYSGOOD is enabled to 
 * ensure it only processes actual syscall boundaries.
 */
void enter_or_exit_syscall(struct parser_ctx_struct *ctx, struct user_regs_struct *regs);
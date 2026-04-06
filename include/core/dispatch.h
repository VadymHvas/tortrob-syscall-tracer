#pragma once

#include <sys/user.h>
#include <sys/types.h>

/**
 * entry_or_exit_syscall - High-level dispatcher for intercepted system calls.
 * @regs:   Pointer to the current CPU register state of the tracee.
 * @tracee: Tracee process PID.
 * 
 * This function acts as the primary logic gate for the tracer. It tracks 
 * whether the process is entering or exiting a system call by maintaining 
 * an internal state.
 * * - On Entry: Extracts and prints syscall name and arguments.
 * - On Exit:  Extracts and prints the return value from the RAX register.
 *
 * Note: This function assumes that PTRACE_O_TRACESYSGOOD is enabled to 
 * ensure it only processes actual syscall boundaries.
 */
void entry_or_exit_syscall(struct user_regs_struct *regs, pid_t tracee);
#pragma once

#include <sys/user.h>
#include <sys/types.h>

/**
 * get_syscall_with_args - Translate raw register state into a formatted string.
 * @regs:    Pointer to the child process registers (captured at syscall entry).
 * @buf:     Output buffer where the formatted string will be stored.
 * @bufsize: Maximum size of the output buffer to prevent overflows.
 *
 * Return: 0 on success, or -1 if the syscall number is unknown or invalid.
 */
int get_syscall_with_args(struct user_regs_struct *regs, char *buf, size_t bufsize);
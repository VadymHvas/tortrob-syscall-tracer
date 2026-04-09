#pragma once

#include <sys/user.h>
#include <sys/types.h>

#include "parser/syscalls/args/helpers.h"

/**
 * get_syscall_with_args - Translate raw register state into a formatted string.
 * @ctx:  Parser context.
 * @regs: Pointer to the child process registers (captured at syscall entry).
 *
 * Return: 0 on success, or -1 if the syscall number is unknown or invalid.
 */
int get_syscall_with_args(struct parser_ctx_struct *ctx, struct user_regs_struct *regs);
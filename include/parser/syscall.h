#pragma once

#include <sys/user.h>
#include <sys/types.h>

#include "parser/syscalls/args/helpers.h"

/**
 * begin_syscall_fmt - Initialize syscall formatting for a traced process.
 * @ctx:  Parser context holding formatting buffer and state.
 * @regs: Pointer to the tracee's register set captured at syscall entry.
 *
 * This function performs syscall identification and prepares the initial
 * textual representation of the syscall (name and opening part of arguments).
 * It does NOT finalize output (no closing parenthesis or return value).
 *
 * Internally, it:
 *  - Resolves the syscall number to a known syscall entry
 *  - Extracts syscall arguments from registers
 *  - Formats syscall name and argument prefix into ctx->buf
 *
 * Return: 0 on success, or -1 if the syscall number is unknown or invalid.
 */
int begin_syscall_fmt(struct parser_ctx_struct *ctx, struct user_regs_struct *regs);

/**
 * finalize_syscall_fmt - Complete syscall formatting and produce final output.
 * @ctx:  Parser context holding the partially constructed syscall string.
 * @regs: Pointer to the tracee's register set captured at syscall exit.
 *
 * This function finalizes the textual representation of a syscall after it
 * returns. It appends closing syntax and return value information to the
 * already formatted syscall string stored in ctx->buf, then prepares it for
 * output.
 *
 * Internally, it:
 *  - Retrieves the syscall return value from registers
 *  - Executes exit-phase syscall-specific parsing (if applicable)
 *  - Appends closing parenthesis ")" to the formatted syscall
 *  - Appends return value in the form " = <value>"
 *
 * Return: 0 on success, or -1 if the syscall number is unknown or invalid.
 */
int finalize_syscall_fmt(struct parser_ctx_struct *ctx, struct user_regs_struct *regs);
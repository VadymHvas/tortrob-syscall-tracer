#pragma once

#include <stdint.h>
#include <sys/user.h>

/**
 * reg_t - Internal type for raw CPU register values.
 * * On x86_64, registers are 64-bit unsigned integers. 
 * This alias helps distinguish between general numbers and 
 * values extracted directly from the CPU context.
 */
typedef uint64_t reg_t;

/**
 * abi_get_retval - Extract the syscall return value.
 * @regs: Pointer to the child process registers.
 * * Returns the value of the RAX register after a syscall exit.
 * Note: Errors are typically returned as negative values in the 
 * range [-4095, -1].
 */
long abi_get_retval(struct user_regs_struct *regs);

/**
 * abi_get_syscall_args - Map CPU registers to a syscall argument array.
 * @regs: Pointer to the child process registers.
 * @args: Output array where the 6 arguments will be stored.
 * * On x86_64 Linux ABI, arguments are passed in the following order:
 * RDI, RSI, RDX, R10, R8, R9.
 */
void abi_get_syscall_args(struct user_regs_struct *regs, reg_t args[]);
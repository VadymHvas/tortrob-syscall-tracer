/*
 * parser/syscall/ABI/abi.c - ABI layer.
 * 
 * This module provides an interface 
 * that is a layer above the system call ABI.
 */

#include <stddef.h>
#include <sys/user.h>
#include <sys/types.h>

#include "parser/syscalls/ABI/abi.h"

#define GET_REG_OFFSET(reg) offsetof(struct user_regs_struct, reg)

/*
 * System V x86_64 ABI:
 *      1st arg: %rdi
 *      2nd arg: %rsi
 *      3rd arg: %rdx
 *      4th arg: %r10
 *      5th arg: %r8
 *      6th arg: %r9
 * 
 *      ABI_REGS_OFFSET[0]: return value (%rax)
 */
static const size_t ABI_REGS_OFFSET[] = {
        [0] = GET_REG_OFFSET(rax),
        [1] = GET_REG_OFFSET(rdi),
        [2] = GET_REG_OFFSET(rsi),
        [3] = GET_REG_OFFSET(rdx),
        [4] = GET_REG_OFFSET(r10),
        [5] = GET_REG_OFFSET(r8),
        [6] = GET_REG_OFFSET(r9)
};

/*
 * Gets the value from the system call argument register,
 * by its number. According to the System V ABI. 
 * 
 *      arg = regs + offset_of_abi_reg[nr_arg]
 */
static raw_reg __abi_get_syscall_arg(struct user_regs_struct *regs, int nr_arg)
{
        if (nr_arg < 0 || nr_arg > 6)
                return 0;

        return *(raw_reg *)((char *)regs + ABI_REGS_OFFSET[nr_arg]);
}

raw_reg abi_get_retval(struct user_regs_struct *regs)
{
        return __abi_get_syscall_arg(regs, 0);
}

void abi_get_syscall_args(struct user_regs_struct *regs, raw_reg args[])
{
        for (int i = 1; i <= 6; i++)
                args[i - 1] = __abi_get_syscall_arg(regs, i);
}
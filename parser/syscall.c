/*
 * parser/syscall.c - Syscall parser.
 *
 * This module provides methods to syscall parsing by it's number.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/user.h>

#include "parser/syscall.h"
#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"

static const struct syscall_entry *__get_syscall_by_nr(long nr);

static int __fmt_syscall_name(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset);
static int __fmt_syscall_args(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset, raw_reg args[]);
static int __fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, raw_reg args[]);

int get_syscall_with_args(struct user_regs_struct *regs, char *buf, size_t bufsize)
{
        const struct syscall_entry *syscall = __get_syscall_by_nr(regs->orig_rax);
        raw_reg args[6];
        
        if (!syscall)
                return -1;
        
        abi_get_syscall_args(regs, args);
        __fmt_syscall(buf, bufsize, syscall, args);

        return 0;
}

static int __fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, raw_reg args[])
{
        size_t offset = 0;

        if (__fmt_syscall_name(buf, bufsize, syscall, &offset))
                return 1;

        return __fmt_syscall_args(buf, bufsize, syscall, &offset, args);
}

static const struct syscall_entry *__get_syscall_by_nr(long nr)
{
        if (nr >= 0 && nr < SYSCALL_COUNT)
                return &syscall_table[nr];

        return NULL;
}

static int __fmt_syscall_name(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset)
{
        *offset += snprintf(buf + (*offset), bufsize - (*offset), "%s(", syscall->name);

        if (*offset >= bufsize)
                return 1;
        
        return 0;
}

static int __fmt_syscall_args(char *buf, size_t bufsize,
        const struct syscall_entry *syscall, size_t *offset, raw_reg args[])
{
        for (int i = 0; i < syscall->args; i++) {
                if (*offset >= bufsize)
                        break;

                const char *sep = (i == 0) ? "" : ", "; 

                *offset += snprintf(buf + (*offset), bufsize - (*offset), "%s%llu", sep, args[i]);
        }

        if (*offset < bufsize)
                snprintf(buf + (*offset), bufsize - (*offset), ")");

        return 0;
}
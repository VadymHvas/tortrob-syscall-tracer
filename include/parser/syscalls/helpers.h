#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, raw_reg args[]);

int fmt_string(char *buf, size_t bufsize, size_t *offset, char *src, int *n);
int fmt_int(char *buf, size_t bufsize, size_t *offset, int num, int *n);
int fmt_addr(char *buf, size_t bufsize, size_t *offset, unsigned long long addr, int *n);
int fmt_separator(char *buf, size_t bufsize, size_t *offset, int *n);
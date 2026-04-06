#include <stdio.h>
#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/helpers.h"

/* Format syscall string function decomposition. */
static int fmt_syscall_name(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset);
static int fmt_syscall_args(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset, raw_reg args[]);

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, raw_reg args[])
{
        size_t offset = 0;

        if (fmt_syscall_name(buf, bufsize, syscall, &offset))
                return 1;

        return fmt_syscall_args(buf, bufsize, syscall, &offset, args);
}

int fmt_string(char *buf, size_t bufsize, size_t *offset, char *src, int *n)
{
        *n = snprintf(buf + *offset, bufsize - *offset, "%s", src);

        if (n < 0) 
                return 1;
                
        *offset += *n;
        if (*offset >= bufsize)
                return 1;

        return 0;
}

int fmt_int(char *buf, size_t bufsize, size_t *offset, int num, int *n)
{
        *n = snprintf(buf + *offset, bufsize - *offset, "%d", num);

        if (*n < 0) 
                return 1;

        *offset += *n;
        if (*offset >= bufsize)
                return 1;

        return 0;
}

int fmt_addr(char *buf, size_t bufsize, size_t *offset, unsigned long long addr, int *n)
{
        *n = snprintf(buf + *offset, bufsize - *offset, "0x%llx",addr);

        if (*n < 0) 
                return 1;

        *offset += *n;
        if (*offset >= bufsize)
                return 1;

        return 0;
}

int fmt_separator(char *buf, size_t bufsize, size_t *offset, int *n)
{
        *n = snprintf(buf + *offset, bufsize - *offset, ", ");

        if (n < 0)
                return 1;

        *offset += *n;

        if (*offset >= bufsize) 
                *offset = bufsize;
        
        return 0;
}

static int fmt_syscall_name(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset)
{
        *offset += snprintf(buf + (*offset), bufsize - (*offset), "%s(", syscall->name);

        if (*offset >= bufsize)
                return 1;
        
        return 0;
}

static int fmt_syscall_args(char *buf, size_t bufsize,
        const struct syscall_entry *syscall, size_t *offset, raw_reg args[])
{
        if (syscall->args)
                syscall_parse(buf, bufsize, syscall, offset, args);

        if (*offset < bufsize)
                snprintf(buf + (*offset), bufsize - (*offset), ")");

        return 0;
}


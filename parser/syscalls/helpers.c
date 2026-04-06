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

int fmt_string(struct parser_ctx_struct *ctx, char *src, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "%s", src);

        if (*n < 0) 
                return 1;
                
        *(ctx->offset) += *n;
        if (*(ctx->offset) >= ctx->bufsize)
                return 1;

        return 0;
}

int fmt_int(struct parser_ctx_struct *ctx, int num, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "%d", num);

        if (*n < 0) 
                return 1;

        *(ctx->offset) += *n;
        if (*(ctx->offset) >= ctx->bufsize)
                return 1;

        return 0;
}

int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "0x%llx",addr);

        if (*n < 0) 
                return 1;

         *(ctx->offset) += *n;
        if (*(ctx->offset) >= ctx->bufsize)
                return 1;

        return 0;
}

int fmt_separator(struct parser_ctx_struct *ctx, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), ", ");

        if (n < 0)
                return 1;

         *(ctx->offset) += *n;
        if (*(ctx->offset) >= ctx->bufsize)
                return 1;
        
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


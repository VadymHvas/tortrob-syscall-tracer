#include <stdio.h>
#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/helpers.h"
#include "parser/syscalls/parser.h"

/* Format syscall string function decomposition. */
static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall);
static int fmt_syscall_args(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[]);
static inline int safe_append(struct parser_ctx_struct *ctx, int *n);

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, pid_t tracee, raw_reg args[])
{
        size_t offset = 0;
        INIT_PARSER_CTX(ctx, buf, bufsize, &offset, tracee);

        if (fmt_syscall_name(&ctx, syscall))
                return 1;

        return fmt_syscall_args(&ctx, syscall, args);
}

int fmt_string(struct parser_ctx_struct *ctx, char *src, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "%s", src);

        return safe_append(ctx, n);
}

int fmt_int(struct parser_ctx_struct *ctx, int num, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "%d", num);

        return safe_append(ctx, n);
}

int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "0x%llx",addr);

        return safe_append(ctx, n);
}

int fmt_separator(struct parser_ctx_struct *ctx, int *n)
{
        *n = snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), ", ");

        return safe_append(ctx, n);
}

static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall)
{
        *(ctx->offset) += snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "%s(", syscall->name);

        if (*(ctx->offset) >= ctx->bufsize)
                return 1;
        
        return 0;
}

static int fmt_syscall_args(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[])
{
        if (syscall->args)
                syscall_parse(ctx, syscall, args);

        if (*(ctx->offset) < ctx->bufsize)
                snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), ")");

        return 0;
}

static inline int safe_append(struct parser_ctx_struct *ctx, int *n)
{
        if (n < 0)
                return 1;

         *(ctx->offset) += *n;
        if (*(ctx->offset) >= ctx->bufsize)
                return 1;
        
        return 0;
}
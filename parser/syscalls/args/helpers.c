#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/fs.h"
#include "core/trace.h"

/* Format syscall string function decomposition. */
static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall);
static int fmt_syscall_args(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[]);
static inline int safe_append(struct parser_ctx_struct *ctx, int *n);
static void escape_seq_parse(const char *src, char *dest, size_t dst_size);
static DEFINE_FMT(null);

#define APPEND_FMT(ctx, fmt, ...) \
        int n = snprintf(ctx->buf + ctx->offset, \
                         ctx->bufsize - ctx->offset, \
                         fmt, __VA_ARGS__); \
        return safe_append(ctx, &n)

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, pid_t tracee, raw_reg args[])
{
        INIT_PARSER_CTX(ctx, buf, bufsize, 0, tracee, 0);

        if (fmt_syscall_name(&ctx, syscall))
                return 1;

        return fmt_syscall_args(&ctx, syscall, args);
}

DEFINE_FMT(string, char *src)
{
        APPEND_FMT(ctx, "%s", src);
}

DEFINE_FMT(int, int num)
{
        APPEND_FMT(ctx, "%d", num);
}

DEFINE_FMT(llu, unsigned long long num)
{
        APPEND_FMT(ctx, "%llu", num);
}

DEFINE_FMT(oct, int num)
{
        APPEND_FMT(ctx, "%o", num);
}

DEFINE_FMT(addr, unsigned long long addr)
{
        APPEND_FMT(ctx, "0x%llx", addr);
}

DEFINE_FMT(string_from_mem, unsigned long long addr, size_t size)
{
        if (!addr)
                return fmt_null(ctx);

        int n;
        char buf[size + 1];
        char escaped_buf[2 * size + 1];

        if (read_tracee_mem(ctx->tracee, (void *)addr, buf, size) == -1)
                return 1;

        buf[size] = '\0';

        escape_seq_parse(buf, escaped_buf, 2 * size + 1);

        FMT_STRING(ctx, "\"");
        FMT_STRING(ctx, escaped_buf);
        FMT_STRING(ctx, "\"");

        return 0;
}

DEFINE_FMT(fd, int fd)
{
        if (fd == AT_FDCWD)
                return fmt_string(ctx, "AT_FDCWD");

        if (fd >= 0 && fd <= 2)
                return fmt_string(ctx, STDFD_NAMES[fd]);
        
        return fmt_int(ctx, fd);
}

static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall)
{
        APPEND_FMT(ctx, "%s(", syscall->name);
}

static int fmt_syscall_args(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[])
{
        if (syscall->args)
                syscall_parse(ctx, syscall, args);

        if (ctx->offset < ctx->bufsize)
                snprintf(ctx->buf + ctx->offset, ctx->bufsize - ctx->offset, ")");

        return 0;
}

static inline int safe_append(struct parser_ctx_struct *ctx, int *n)
{
        if (n < 0)
                return 1;

        ctx->offset += *n;
        if (ctx->offset >= ctx->bufsize)
                return 1;
        
        return 0;
}

static void escape_seq_parse(const char *src, char *dest, size_t dst_size)
{
        size_t i = 0;

        while (src && i < dst_size - 1) {
                if (*src == '\n') {
                        if (i + 2 >= dst_size)
                                break;

                        dest[i++] = '\\';
                        dest[i++] = 'n';
                } else if (*src == '\t') {
                        if (i + 2 >= dst_size)
                                break;
                        
                        dest[i++] = '\\';
                        dest[i++] = 't';
                } else {
                        dest[i++] = *src;
                }

                src++;
        }

        dest[i] = '\0';
}

static DEFINE_FMT(null)
{
        APPEND_FMT(ctx, "%s", "NULL");
}
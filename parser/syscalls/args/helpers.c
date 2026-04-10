#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sysmacros.h>
#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/fs.h"
#include "parser/syscalls/args/struct_info.h"
#include "core/trace.h"

#define TRACE_MAX_STRLEN 48

#define APPEND_FMT(ctx, fmt, ...) \
        int n = snprintf(ctx->buf + ctx->offset, \
                         ctx->bufsize - ctx->offset, \
                         fmt, __VA_ARGS__); \
        return safe_append(ctx, &n)

/* Format syscall string function decomposition. */
static DEFINE_FMT(syscall_name, const struct syscall_entry *syscall);
static DEFINE_FMT(syscall_args, const struct syscall_entry *syscall, raw_reg args[]);

static inline int safe_append(struct parser_ctx_struct *ctx, int *n);
static void escape_seq_parse(const char *src, char *dest, size_t dst_size);

DEFINE_FMT(syscall, const struct syscall_entry *syscall, raw_reg args[])
{
        if (!ctx->delayed)
                if (fmt_syscall_name(ctx, syscall))
                        return 1;

        return fmt_syscall_args(ctx, syscall, args);
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

DEFINE_FMT(off, off_t num)
{
        APPEND_FMT(ctx, "%lld", num);
}

DEFINE_FMT(hex, int num)
{
        APPEND_FMT(ctx, "0x%x", num);
}

DEFINE_FMT(addr, unsigned long long addr)
{
        APPEND_FMT(ctx, "0x%llx", addr);
}

DEFINE_FMT(dev, dev_t dev)
{
        APPEND_FMT(ctx, "%d:%d", major(dev), minor(dev));
}

DEFINE_FMT(string_from_mem, unsigned long long addr, size_t size)
{
        if (!addr) 
                return fmt_null(ctx);

        size_t max_len = (size > TRACE_MAX_STRLEN) ? TRACE_MAX_STRLEN : size; 

        char *buf = malloc(max_len + 1);
        char *escaped_buf = malloc(max_len * 4 + 1);

        if (!buf || !escaped_buf) {
                if (buf) 
                        free(buf);
                if (escaped_buf) 
                        free(escaped_buf);
                return 1;
        }

        if (read_tracee_mem(ctx->tracee, (void *)addr, buf, max_len) <= 0)
                goto err;

        escape_seq_parse(buf, escaped_buf, max_len * 4 + 1);

        size_t escaped_buf_len = strnlen(escaped_buf, max_len); // Bytes to format.

        if (fmt_string(ctx, "\"") || fmt_string(ctx, escaped_buf))
                goto err;

        if (escaped_buf_len == max_len) {
                if (fmt_string(ctx, "\"...")) 
                        goto err;
        } else {
                if (fmt_string(ctx, "\"")) 
                        goto err;
        }

        free(buf);
        free(escaped_buf);
        return 0;

err:
        free(buf);
        free(escaped_buf);
        return 1;
}

DEFINE_FMT(fd, int fd)
{
        if (fd == AT_FDCWD)
                return fmt_string(ctx, "AT_FDCWD");

        if (fd >= 0 && fd <= 2)
                return fmt_string(ctx, STDFD_NAMES[fd]);
        
        return fmt_int(ctx, fd);
}

DEFINE_FMT(null)
{
        APPEND_FMT(ctx, "%s", "NULL");
}

static DEFINE_FMT(syscall_name, const struct syscall_entry *syscall)
{
        APPEND_FMT(ctx, "%s(", syscall->name);
}

static DEFINE_FMT(syscall_args, const struct syscall_entry *syscall, raw_reg args[])
{
        if (syscall->args)
                syscall_parse(ctx, syscall, args);

        if (ctx->offset < ctx->bufsize && !ctx->delayed)
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
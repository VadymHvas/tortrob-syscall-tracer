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

static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall);

static inline int safe_append(struct parser_ctx_struct *ctx, int *n);
static void escape_seq_parse(const char *src, char *dest, size_t dst_size);

int fmt_syscall_enter(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, reg_t args[])
{
        TRY_FMT(fmt_syscall_name, ctx, syscall);

        if (syscall->args)
                syscall_parse(ctx, syscall, args);
        
        return 0;
}

int fmt_string(struct parser_ctx_struct *ctx, char *src)
{
        APPEND_FMT(ctx, "%s", src);
}

int fmt_int(struct parser_ctx_struct *ctx, int num)
{
        APPEND_FMT(ctx, "%d", num);
}

int fmt_llu(struct parser_ctx_struct *ctx, unsigned long long num)
{
        APPEND_FMT(ctx, "%llu", num);
}

int fmt_oct(struct parser_ctx_struct *ctx,  int num)
{
        APPEND_FMT(ctx, "%o", num);
}

int fmt_off(struct parser_ctx_struct *ctx, off_t num)
{
        APPEND_FMT(ctx, "%lld", num);
}

int fmt_hex(struct parser_ctx_struct *ctx, int num)
{
        APPEND_FMT(ctx, "0x%x", num);
}

int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr)
{
        APPEND_FMT(ctx, "0x%llx", addr);
}

int fmt_dev(struct parser_ctx_struct *ctx, dev_t dev)
{
        APPEND_FMT(ctx, "%d:%d", major(dev), minor(dev));
}

int fmt_string_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr, size_t size)
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

        buf[max_len] = '\0';

        escape_seq_parse(buf, escaped_buf, max_len * 4 + 1);
        
        size_t escaped_buf_len = strnlen(escaped_buf, max_len); // Bytes to format.

        if (fmt_string(ctx, "\"") || fmt_string(ctx, escaped_buf))
                goto err;

        if (escaped_buf_len == TRACE_MAX_STRLEN) {
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

int fmt_fd(struct parser_ctx_struct *ctx, int fd)
{
        if (fd == AT_FDCWD)
                return fmt_string(ctx, "AT_FDCWD");

        if (fd >= 0 && fd <= 2)
                return fmt_string(ctx, STDFD_NAMES[fd]);
        
        return fmt_int(ctx, fd);
}

int fmt_null(struct parser_ctx_struct *ctx)
{
        APPEND_FMT(ctx, "%s", "NULL");
}

static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall)
{
        APPEND_FMT(ctx, "%s(", syscall->name);
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
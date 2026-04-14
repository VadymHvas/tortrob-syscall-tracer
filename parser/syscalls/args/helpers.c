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
#define TRACE_MAX_BUFLEN 48

#define APPEND_FMT(ctx, fmt, ...) \
        int n = snprintf(ctx->buf + ctx->offset, \
                         ctx->bufsize - ctx->offset, \
                         fmt, __VA_ARGS__); \
        return safe_append(ctx, &n)

static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall);

static inline int safe_append(struct parser_ctx_struct *ctx, int *n);
static void escape_seq_parse(const char *src, size_t src_len, char *dest, size_t dst_size);

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

int fmt_bytes_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr, size_t size)
{
        if (!addr || !size)
                return fmt_null(ctx);

        size_t max_len = (size > TRACE_MAX_BUFLEN) ? TRACE_MAX_BUFLEN : size;
        
        char raw_buf[max_len + 1];
        char escaped_buf[max_len * 4 + 1];

        ssize_t bytes_read = read_tracee_mem(ctx->tracee, (void *)addr, raw_buf, max_len);
        if (bytes_read <= 0)
                return fmt_string(ctx, "<failed>");

        if (bytes_read > max_len)
                bytes_read = max_len;

        escape_seq_parse(raw_buf, bytes_read, escaped_buf, sizeof(escaped_buf));

        size_t escaped_len = strnlen(escaped_buf, sizeof(escaped_buf));

        FMT_STRING(ctx, "\"");
        FMT_STRING(ctx, escaped_buf);
        FMT_STRING(ctx, "\"");

        int truncate = (bytes_read == TRACE_MAX_BUFLEN);

        if (truncate)
                FMT_STRING(ctx, "...");

        return 0;
}

int fmt_cstring_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr)
{
        if (!addr)
                return fmt_null(ctx);

        size_t total = 0;
        const size_t chunk_size = 16;

        char raw_buf[TRACE_MAX_STRLEN + 1];
        char tmp[chunk_size];

        while (total < TRACE_MAX_STRLEN) {
                size_t to_read = chunk_size;

                if (total + to_read > TRACE_MAX_STRLEN)
                        to_read = TRACE_MAX_STRLEN - total;

                ssize_t n = read_tracee_mem(ctx->tracee, (void *)(addr + total), tmp, to_read);

                if (n <= 0)
                        return fmt_string(ctx, "<failed>");

                for (ssize_t i = 0; i < n; i++) {
                        raw_buf[total++] = tmp[i];

                        if (tmp[i] == '\0') {
                                raw_buf[total] = '\0';
                                goto done;
                        }

                        if (total == TRACE_MAX_STRLEN)
                                break;
                }

                if ((size_t)n < to_read)
                        break;
        }

done:
        raw_buf[total] = '\0';

        char escaped_buf[TRACE_MAX_STRLEN * 4 + 1];
        size_t actual_len = strnlen(raw_buf, total);

        escape_seq_parse(raw_buf, actual_len, escaped_buf, sizeof(escaped_buf));

        FMT_STRING(ctx, "\"");
        FMT_STRING(ctx, escaped_buf);
        FMT_STRING(ctx, "\"");

        if (total == TRACE_MAX_STRLEN && raw_buf[total - 1] != '\0')
                FMT_STRING(ctx, "...");

        return 0;
}

int fmt_word_mem(struct parser_ctx_struct *ctx, unsigned long long addr)
{
        if (!addr)
                return fmt_null(ctx);

        long word;

        if (read_tracee_word(ctx->tracee, addr, &word) == -1)
                return fmt_string(ctx, "<failed>");

        FMT_STRING(ctx, "[");
        FMT_LLU(ctx, word);
        FMT_STRING(ctx, "]");        

        return 0;
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

static void escape_seq_parse(const char *src, size_t src_len, char *dest, size_t dst_size)
{
    size_t d_i = 0;

    for (size_t s = 0; s < src_len; s++) {
        if (d_i + 1 >= dst_size) 
                break;

        unsigned char c = (unsigned char)src[s];

        switch (c) 
        {
        case '\n':
                if (d_i + 2 < dst_size) { dest[d_i++] = '\\'; dest[d_i++] = 'n'; }
                break;
        case '\r':
                if (d_i + 2 < dst_size) { dest[d_i++] = '\\'; dest[d_i++] = 'r'; }
                break;
        case '\t':
                if (d_i + 2 < dst_size) { dest[d_i++] = '\\'; dest[d_i++] = 't'; }
        break;
        case '\\':
                if (d_i + 2 < dst_size) { dest[d_i++] = '\\'; dest[d_i++] = '\\'; }
                break;
        case '\"':
                if (d_i + 2 < dst_size) { dest[d_i++] = '\\'; dest[d_i++] = '\"'; }
                break;

        default:
                if (c >= 32 && c <= 126) {
                        dest[d_i++] = c;
                } 
                else {
                        if (d_i + 4 < dst_size) {
                                snprintf(&dest[d_i], 5, "\\x%02x", c);
                                d_i += 4;
                        }
                }
                break;
        }
    }

    dest[d_i] = '\0';
}
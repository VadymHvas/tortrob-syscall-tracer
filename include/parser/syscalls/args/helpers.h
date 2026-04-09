#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/args/struct_info.h"
#include "core/dispatch.h"

/**
 * DEFINE_FMT(name, ...) - Define formatter function.
 * 
 * Helper that defines formatter function with 
 * specified params in __VA_ARGS__.
 */
#define DEFINE_FMT(name, ...) \
        int fmt_##name(struct parser_ctx_struct *ctx, ##__VA_ARGS__)

/**
 * TRY_FMT(fn_call, ctx, ...) - Try to format argument.
 * 
 * Helper macro that tries to format an argument using a function
 * fn_call, with arguments ctx, args.
 */
#define TRY_FMT(fn_call, ctx, ...) \
        if (fn_call(ctx, __VA_ARGS__)) \
                return 1

#define FMT_STRING(ctx, str) TRY_FMT(fmt_string, ctx, str)
#define FMT_INT(ctx, num)    TRY_FMT(fmt_int, ctx, num)
#define FMT_LLU(ctx, num)    TRY_FMT(fmt_llu, ctx, num)
#define FMT_OCT(ctx, num)    TRY_FMT(fmt_oct, ctx, num)
#define FMT_OFF(ctx, num)    TRY_FMT(fmt_off, ctx, num)
#define FMT_HEX(ctx, num)    TRY_FMT(fmt_hex, ctx, num)

#define FMT_ADDR(ctx, addr)  TRY_FMT(fmt_addr, ctx, addr)
#define FMT_FD(ctx, fd)      TRY_FMT(fmt_fd, ctx, fd)
#define FMT_SEPARATOR(ctx)   TRY_FMT(fmt_string, ctx, ", ")

#define FMT_STRING_MEM(ctx, addr, size) TRY_FMT(fmt_string_from_mem, ctx, addr, size)

#define INIT_PARSER_CTX(var, buf, bufsize, tracee) \
        struct parser_ctx_struct ctx = { SYSCALL_ENTRY, buf, .buf_base = buf, SYSCALL_BUF_SIZE, 0, tracee, 0 }

#define CLEANUP_PARSER_CTX(ctx) \
        ctx->in_syscall = SYSCALL_ENTRY; \
        ctx->buf        = ctx->buf_base; \
        ctx->offset     = 0; \
        ctx->extra      = 0;

/**
 * struct parser_ctx_struct - Parser context.
 * 
 * @in_syscall: Parser syscall state.
 * @buf:        Buffer addr.
 * @buf_base:    Buffer base addr (for cleanup buffer).
 * @bufsize:    Max size of buffer.
 * @offset:     Pointer to offset variable.
 * @tracee:     Tracee process PID.
 * @extra:      Field for special cases, for instance fcntl(), prctl(), ioctl() etc.
 * 
 * This structure stores all the necessary
 * information for correct parsing using offsets.
 */
struct parser_ctx_struct {
        int in_syscall;
        char *buf;
        const char *buf_base;
        size_t bufsize;
        size_t offset;
        pid_t tracee;
        int extra;
};

typedef (*fmt_struct_func_t) (struct parser_ctx_struct *ctx, void *st);

DEFINE_FMT(syscall, const struct syscall_entry *syscall, raw_reg args[]);

DEFINE_FMT(string, char *src);
DEFINE_FMT(int, int num);
DEFINE_FMT(llu, unsigned long long num);
DEFINE_FMT(oct, int num);
DEFINE_FMT(off, off_t num);
DEFINE_FMT(hex, int num);
DEFINE_FMT(addr, unsigned long long addr);
DEFINE_FMT(string_from_mem, unsigned long long addr, size_t size);
DEFINE_FMT(struct_common, fmt_struct_func_t fmt_struct_func, unsigned long long addr, size_t size);
DEFINE_FMT(fd, int fd);
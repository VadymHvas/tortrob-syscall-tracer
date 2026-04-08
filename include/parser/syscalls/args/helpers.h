#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"

/**
 * INIT_PARSER_CTX(var, buf, bufsize, offset) - Initialization of parser context.
 * 
 * Helper macro that initializes 
 * the struct parser_ctx_struct and fills it with values.
 */
#define INIT_PARSER_CTX(var, buf, bufsize, offset, tracee, extra) \
        struct parser_ctx_struct var = { buf, bufsize, offset, tracee, extra };

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
#define FMT_ADDR(ctx, addr)  TRY_FMT(fmt_addr, ctx, addr)
#define FMT_FD(ctx, fd)      TRY_FMT(fmt_fd, ctx, fd)
#define FMT_SEPARATOR(ctx)   TRY_FMT(fmt_string, ctx, ", ")

#define FMT_STRING_MEM(ctx, addr, size) TRY_FMT(fmt_string_from_mem, ctx, addr, size)

/**
 * struct parser_ctx_struct - Parser context.
 * 
 * @buf:     Buffer addr.
 * @bufsize: Max size of buffer.
 * @offset:  Pointer to offset variable.
 * @tracee:  Tracee process PID.
 * @extra:   Field for special cases, for instance fcntl(), prctl(), ioctl() etc.
 * 
 * This structure stores all the necessary
 * information for correct parsing using offsets.
 */
struct parser_ctx_struct {
        char *buf;
        size_t bufsize;
        size_t offset;
        pid_t tracee;
        int extra;
};

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, pid_t tracee, raw_reg args[]);

int fmt_string(struct parser_ctx_struct *ctx, char *src);
int fmt_int(struct parser_ctx_struct *ctx, int num);
int fmt_llu(struct parser_ctx_struct *ctx, unsigned long long num);
int fmt_oct(struct parser_ctx_struct *ctx, int num);
int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr);
int fmt_string_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr, size_t size);
int fmt_fd(struct parser_ctx_struct *ctx, int fd);
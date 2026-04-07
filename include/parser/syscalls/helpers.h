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
#define INIT_PARSER_CTX(var, buf, bufsize, offset, tracee) \
        struct parser_ctx_struct var = { buf, bufsize, offset, tracee };

/**
 * TRY_FMT_n(fn_call, ctx, ..., n) - Try to format argument.
 * 
 * Helper macro that tries to format an argument using a function
 * fn_call, with arguments ctx, v, n.
 */
#define TRY_FMT_1(fn_call, ctx, v) \
        if (fn_call(ctx, v)) \
                return 1

#define TRY_FMT_2(fn_call, ctx, v, v2) \
        if (fn_call(ctx, v, v2)) \
                return 1

/**
 * TRY_FMT_WITH_SEP_n(fn_call, ctx, ..., n) - Try to insert separator and format argument.
 * 
 * Helper macro that tries to insert separator and format an argument using a function
 * fn_call, with arguments ctx, v, n.
 */
#define TRY_FMT_WITH_SEP_1(fn_call, ctx, v) \
        TRY_FMT_1(fmt_string, ctx, ", "); \
        TRY_FMT_1(fn_call, ctx, v)

#define TRY_FMT_WITH_SEP_2(fn_call, ctx, v, v2) \
        TRY_FMT_1(fmt_string, ctx, ", "); \
        TRY_FMT_2(fn_call, ctx, v, v2)

/**
 * struct parser_ctx_struct - Parser context.
 * 
 * @buf:     Buffer addr.
 * @bufsize: Max size of buffer.
 * @offset:  Pointer to offset variable.
 * @tracee:  Tracee process PID.
 * 
 * This structure stores all the necessary
 * information for correct parsing using offsets.
 */
struct parser_ctx_struct {
        char *buf;
        size_t bufsize;
        size_t *offset;
        pid_t tracee;
};

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, pid_t tracee, raw_reg args[]);

int fmt_string(struct parser_ctx_struct *ctx, char *src);
int fmt_int(struct parser_ctx_struct *ctx, int num);
int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr);
int fmt_string_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr, size_t size);
int fmt_fd(struct parser_ctx_struct *ctx, int fd);
#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"'

/**
 * INIT_PARSER_CTX(var, buf, bufsize, offset) - Initialization of parser context.
 * 
 * Helper macro that initializes 
 * the struct parser_ctx_struct and fills it with values.
 */
#define INIT_PARSER_CTX(var, buf, bufsize, offset) \
        struct parser_ctx_struct var = { buf, bufsize, offset };

/**
 * TRY_FMT(fn_call, ctx, value, n) - Try to format argument.
 * 
 * Helper macro that tries to format an argument using a function
 * fn_call, with arguments ctx, value, n.
 */
#define TRY_FMT(fn_call, ctx, value, n) \
        if (fn_call(ctx, value, n)) \
                return 1

/**
 * TRY_FMT_WITH_SEP(fn_call, ctx, value, n) - Try to insert separator and format argument.
 * 
 * Helper macro that tries to insert separator and format an argument using a function
 * fn_call, with arguments ctx, value, n.
 */
#define TRY_FMT_WITH_SEP(fn_call, ctx, value, n) \
        if (fmt_separator(ctx, n)) \
                return 1; \
        TRY_FMT(fn_call, ctx, value, n)

/**
 * struct parser_ctx_struct - Parser context.
 * 
 * @buf: Buffer addr.
 * @bufsize: Max size of buffer.
 * @offset: Pointer to offset variable.
 * 
 * This structure stores all the necessary
 * information for correct parsing using offsets.
 */
struct parser_ctx_struct {
        char *buf;
        size_t bufsize;
        size_t *offset;
};

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, raw_reg args[]);

int fmt_string(struct parser_ctx_struct *ctx, char *src, int *n);
int fmt_int(struct parser_ctx_struct *ctx, int num, int *n);
int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr, int *n);
int fmt_separator(struct parser_ctx_struct *ctx, int *n);
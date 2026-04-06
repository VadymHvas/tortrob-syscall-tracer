#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"'

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

#define INIT_PARSER_CTX(var, buf, bufsize, offset) \
        struct parser_ctx_struct var = { buf, bufsize, offset };

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, raw_reg args[]);

int fmt_string(struct parser_ctx_struct *ctx, char *src, int *n);
int fmt_int(struct parser_ctx_struct *ctx, int num, int *n);
int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr, int *n);
int fmt_separator(struct parser_ctx_struct *ctx, int *n);
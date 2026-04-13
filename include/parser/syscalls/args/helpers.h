#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/args/struct_info.h"
#include "core/dispatch.h"

/*
 * TRY_FMT(fmt_call, ctx, ...) - Try to format argument.
 * 
 * Helper macro that tries to format an argument using a function
 * fmt_call, with arguments ctx, args.
 */
#define TRY_FMT(fmt_call, ctx, ...) \
        if (fmt_call(ctx, __VA_ARGS__)) \
                return 1

#define FMT_STRING(ctx, str) TRY_FMT(fmt_string, ctx, str)
#define FMT_INT(ctx, num)    TRY_FMT(fmt_int, ctx, num)
#define FMT_LLU(ctx, num)    TRY_FMT(fmt_llu, ctx, num)
#define FMT_OCT(ctx, num)    TRY_FMT(fmt_oct, ctx, num)
#define FMT_OFF(ctx, num)    TRY_FMT(fmt_off, ctx, num)
#define FMT_HEX(ctx, num)    TRY_FMT(fmt_hex, ctx, num)

#define FMT_ADDR(ctx, addr)  TRY_FMT(fmt_addr, ctx, addr)
#define FMT_DEV(ctx, dev)    TRY_FMT(fmt_dev, ctx, dev)
#define FMT_FD(ctx, fd)      TRY_FMT(fmt_fd, ctx, fd)
#define FMT_SEPARATOR(ctx)   TRY_FMT(fmt_string, ctx, ", ")

#define FMT_STRING_MEM(ctx, addr, size) TRY_FMT(fmt_string_from_mem, ctx, addr, size)
#define FMT_STRING_MEM_IF_OK(ctx, addr, retval) \
        if (retval < 0) { \
                FMT_ADDR(ctx, addr); \
        } else { \
                FMT_STRING_MEM(ctx, addr, retval); \
        }

#define FMT_WORD_MEM(ctx, addr) TRY_FMT(fmt_word_mem, ctx, addr)

#define INIT_PARSER_CTX(var, buffer, buffer_size, tracee_pid) \
        struct parser_ctx_struct var = \
                { \
                        .in_syscall = SYSCALL_ENTER, \
                        .buf        = buffer, \
                        .bufsize    = buffer_size, \
                        .offset     = 0, \
                        .tracee     = tracee_pid, \
                        .extra      = 0, \
                        .retval     = 0 \
                }

/**
 * struct parser_ctx_struct - Parser context.
 * 
 * @in_syscall: Parser syscall state.
 * @buf:        Buffer addr.
 * @bufsize:    Max size of buffer.
 * @offset:     Pointer to offset variable.
 * @tracee:     Tracee process PID.
 * @extra:      Field for special cases, for instance fcntl(), prctl(), ioctl() etc.
 * @retval:     Return value (is set when exit syscall).      
 * 
 * This structure stores all the necessary
 * information for correct parsing using offsets.
 */
struct parser_ctx_struct {
        int in_syscall;

        char *buf;
        size_t bufsize;
        size_t offset;

        pid_t tracee;

        int extra;

        long retval;
};

int fmt_syscall_enter(struct parser_ctx_struct *ctx, 
                      const struct syscall_entry *syscall, reg_t args[]);
                      
int fmt_string(struct parser_ctx_struct *ctx, char *src);
int fmt_int(struct parser_ctx_struct *ctx, int num);
int fmt_llu(struct parser_ctx_struct *ctx, unsigned long long num);
int fmt_oct(struct parser_ctx_struct *ctx,  int num);
int fmt_off(struct parser_ctx_struct *ctx, off_t num);
int fmt_hex(struct parser_ctx_struct *ctx, int num);
int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr);
int fmt_dev(struct parser_ctx_struct *ctx, dev_t dev);
int fmt_string_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr, size_t size);
int fmt_word_mem(struct parser_ctx_struct *ctx, unsigned long long addr);
int fmt_fd(struct parser_ctx_struct *ctx, int fd);
int fmt_null(struct parser_ctx_struct *ctx);
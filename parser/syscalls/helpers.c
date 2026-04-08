#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/helpers.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/fs.h"
#include "core/trace.h"

static const struct flag_info open_flags[] = {
        { O_CREAT,     "O_CREAT" },
        { O_EXCL,      "O_EXECL" },
        { O_TRUNC,     "O_TRUNC" },
        { O_APPEND,    "O_APPEND" },
        { O_NONBLOCK,  "O_NONBLOCK" },
        { O_DSYNC,     "O_DSYNC" },
        { O_SYNC,      "O_SYNC" },
        { O_RSYNC,     "O_RSYNC" },
        { O_CLOEXEC,   "O_CLOEXEC" },
        { O_DIRECTORY, "O_DIRECTORY" },
        { O_NOFOLLOW,  "O_NOFOLLOW" },
        { O_TMPFILE,   "O_TMPFILE" }
};

static const struct flag_info at_flags[] = {
        { AT_SYMLINK_FOLLOW,   "AT_SYMLINK_FOLLOW" },
        { AT_SYMLINK_NOFOLLOW, "AT_SYMLINK_NOFOLLOW" },
        { AT_REMOVEDIR,        "AT_REMOVEDIR" },
        { AT_NO_AUTOMOUNT,     "AT_NO_AUTOMOUNT" },
        { AT_EMPTY_PATH,       "AT_EMPTY_PATH" },
};

static const struct flag_info renameat2_flags[] = {
        { RENAME_NOREPLACE, "RENAME_NOREPLACE" },
        { RENAME_EXCHANGE,  "RENAME_EXCHANGE" },
        { RENAME_WHITEOUT,  "RENAME_WHITEOUT" }
};

/* Format syscall string function decomposition. */
static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall);
static int fmt_syscall_args(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[]);
static inline int safe_append(struct parser_ctx_struct *ctx, int *n);
static void escape_seq_parse(const char *src, char *dest, size_t dst_size);

#define APPEND_FMT(ctx, fmt, ...) \
        int n = snprintf(ctx->buf + *(ctx->offset), \
                         ctx->bufsize - *(ctx->offset), \
                         fmt, __VA_ARGS__); \
        return safe_append(ctx, &n)

int fmt_syscall(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, pid_t tracee, raw_reg args[])
{
        size_t offset = 0;
        INIT_PARSER_CTX(ctx, buf, bufsize, &offset, tracee);

        if (fmt_syscall_name(&ctx, syscall))
                return 1;

        return fmt_syscall_args(&ctx, syscall, args);
}

int fmt_string(struct parser_ctx_struct *ctx, char *src)
{
        APPEND_FMT(ctx, "%s", src);
}

int fmt_int(struct parser_ctx_struct *ctx, int num)
{
        APPEND_FMT(ctx, "%d", num);
}

int fmt_oct(struct parser_ctx_struct *ctx, int num)
{
        APPEND_FMT(ctx, "%o", num);
}

int fmt_addr(struct parser_ctx_struct *ctx, unsigned long long addr)
{
        APPEND_FMT(ctx, "0x%llx", addr);
}

int fmt_string_from_mem(struct parser_ctx_struct *ctx, unsigned long long addr, size_t size)
{
        int n;
        char buf[size + 1];
        char escaped_buf[2 * size + 1];

        if (read_tracee_mem(ctx->tracee, (void *)addr, buf, size) == -1)
                return 1;

        buf[size] = '\0';

        escape_seq_parse(buf, escaped_buf, 2 * size + 1);

        TRY_FMT(fmt_string, ctx, "\"");
        TRY_FMT(fmt_string, ctx, escaped_buf);
        TRY_FMT(fmt_string, ctx, "\"");

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

int fmt_open_flags(struct parser_ctx_struct *ctx, int flags)
{
        switch (flags & O_ACCMODE)
        {
        case O_RDONLY: FMT_STRING(ctx, "O_RDONLY"); break;
        case O_WRONLY: FMT_STRING(ctx, "O_WRONLY"); break;
        case O_RDWR: FMT_STRING(ctx, "O_RDWR"); break;
        }

        for (int i = 0; i < sizeof(open_flags) / sizeof(open_flags[0]); i++) {
                if (flags & open_flags[i].flag) {
                        FMT_STRING(ctx, "|");
                        FMT_STRING(ctx, open_flags[i].name);
                }
        }

        return 0;
}

int fmt_at_flags(struct parser_ctx_struct *ctx, int flags)
{
        if (flags == 0) {
                FMT_INT(ctx, 0);
                return 0;
        }

        int first = 1;

        for (int i = 0; i < sizeof(at_flags) / sizeof(at_flags[0]); i++) {
                if (flags & at_flags[i].flag) {
                        if (!first)
                                FMT_STRING(ctx, "|");

                        FMT_STRING(ctx, at_flags[i].name);
                        first = 0;
                }
        }

        if (flags & AT_STATX_SYNC_TYPE) {
                const char *sync_type_name = NULL;

                switch (flags & AT_STATX_SYNC_TYPE) {
                        case AT_STATX_SYNC_AS_STAT: sync_type_name = "AT_STATX_SYNC_AS_STAT"; break;
                        case AT_STATX_FORCE_SYNC:  sync_type_name = "AT_STATX_FORCE_SYNC"; break;
                        case AT_STATX_DONT_SYNC:   sync_type_name = "AT_STATX_DONT_SYNC"; break;
                }

                if (sync_type_name) {
                        if (!first)
                                FMT_STRING(ctx, "|");
                        
                        FMT_STRING(ctx, sync_type_name);
                }

        }

        return 0;
}

int fmt_renameat2_flags(struct parser_ctx_struct *ctx, int flags)
{
        if (flags == 0) {
                FMT_INT(ctx, 0);
                return 0;
        }

        int first = 1;

        for  (int i = 0; i < sizeof(renameat2_flags) / sizeof(renameat2_flags[0]); i++) {
                if (flags & renameat2_flags[i].flag) {
                        if (!first)
                                FMT_STRING(ctx, "|");

                        FMT_STRING(ctx, renameat2_flags[i].name);
                        first = 0;
                }
        }

        return 0;
}

static int fmt_syscall_name(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall)
{
        *(ctx->offset) += snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), "%s(", syscall->name);

        if (*(ctx->offset) >= ctx->bufsize)
                return 1;
        
        return 0;
}

static int fmt_syscall_args(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[])
{
        if (syscall->args)
                syscall_parse(ctx, syscall, args);

        if (*(ctx->offset) < ctx->bufsize)
                snprintf(ctx->buf + *(ctx->offset), ctx->bufsize - *(ctx->offset), ")");

        return 0;
}

static inline int safe_append(struct parser_ctx_struct *ctx, int *n)
{
        if (n < 0)
                return 1;

         *(ctx->offset) += *n;
        if (*(ctx->offset) >= ctx->bufsize)
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
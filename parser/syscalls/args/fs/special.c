#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/file.h>
#include <sys/mount.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/fs/special.h"
#include "parser/syscalls/args/fs/struct.h"

int fmt_fcntl_arg3(struct parser_ctx_struct *ctx, void *arg3)
{
        switch (ctx->extra)
        {
        case FCNTL_FLAGS:
                FMT_OPEN_FLAGS(ctx, *(int *)arg3);
                break;

        case FCNTL_FD_FLAGS:
                FMT_STRING(ctx, "FD_CLOEXEC");
                break;

        case FCNTL_FLOCK:
                FMT_FLOCK_STRUCT(ctx, *(unsigned long long *)arg3);
                break;

        default:
                FMT_INT(ctx, *(int *)arg3);
                break;
        }

        ctx->extra = 0;
        return 0;
}

int fmt_fsconfig_value(struct parser_ctx_struct *ctx, const void *value, int aux)
{
        switch (ctx->extra)
        {
        case FSCONFIG_SET_STRING:
        case FSCONFIG_SET_PATH:
        case FSCONFIG_SET_PATH_EMPTY:
                FMT_CSTRING_MEM(ctx, value);
                break;
        
        case FSCONFIG_SET_FD:
                FMT_FD(ctx, value);
                break;

        case FSCONFIG_SET_FLAG:
                FMT_STRING(ctx, "<flag>");
                break;

        case FSCONFIG_SET_BINARY:
                FMT_BYTES_MEM(ctx, value, aux);
                break;

        case FSCONFIG_CMD_CREATE:
        case FSCONFIG_CMD_CREATE_EXCL:
        case FSCONFIG_CMD_RECONFIGURE:
                fmt_null(ctx);
                break;

        default:
                FMT_STRING(ctx, "<unknown>");
                break;
        } 

        return 0;
}
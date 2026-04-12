#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/file.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/fs/special.h"

int fmt_fcntl_arg3(struct parser_ctx_struct *ctx, int arg3)
{
        switch (ctx->extra)
        {
        case FCNTL_FLAGS:
                FMT_OPEN_FLAGS(ctx, arg3);
                break;

        case FCNTL_FD_FLAGS:
                FMT_STRING(ctx, "FD_CLOEXEC");
                break;

        case FCNTL_FLOCK:
                FMT_ADDR(ctx, arg3);
                break;

        default:
                FMT_INT(ctx, arg3);
                break;
        }

        ctx->extra = 0;
        return 0;
}
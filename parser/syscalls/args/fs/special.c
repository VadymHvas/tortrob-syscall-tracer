#define _GNU_SOURCE

#include <fcntl.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"
#include "parser/syscalls/args/fs/special.h"
#include "parser/syscalls/args/fs/flags.h"

DEFINE_FLAGS_ARRAY(fcntl_cmds) = {
        INIT_FLAG_INFO(F_SETFD),
        INIT_FLAG_INFO(F_SETFL),
        INIT_FLAG_INFO(F_SETLK),
        INIT_FLAG_INFO(F_SETLKW),
        INIT_FLAG_INFO(F_SETOWN),
        INIT_FLAG_INFO(F_SETSIG),
        INIT_FLAG_INFO(F_SETPIPE_SZ),
        INIT_FLAG_INFO(F_DUPFD),
        INIT_FLAG_INFO(F_DUPFD_CLOEXEC),
        INIT_FLAG_INFO(F_GETFD),
        INIT_FLAG_INFO(F_GETFL),
        INIT_FLAG_INFO(F_GETOWN),
        INIT_FLAG_INFO(F_GETSIG),
        INIT_FLAG_INFO(F_GETPIPE_SZ),
        INIT_FLAG_INFO(F_GETLK)
};

/* 
 * For F_GET* commands (except F_GETLK) we don't format 3rd argument, 
 * therefore ctx->extra is equals to FCNTL_IGNORE_ARG3.
 */ 
DEFINE_FMT(fcntl_cmd, int cmd)
{
        if (cmd == F_SETFL)
                ctx->extra = FCNTL_FLAGS;
        else if (cmd == F_SETFD)
                ctx->extra = FCNTL_FD_FLAGS;
        else if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK)
                ctx->extra = FCNTL_FLOCK;
        else if (cmd == F_SETOWN || cmd == F_SETPIPE_SZ || cmd == F_SETSIG)
                ctx->extra = FCNTL_INT;
        else if (cmd == F_GETFD || cmd == F_GETFL ||
                 cmd == F_GETOWN || cmd == F_GETPIPE_SZ ||
                 cmd == F_GETSIG)
                ctx->extra = FCNTL_IGNORE_ARG3;

        FOR_EACH_SPECIAL(fcntl_cmds) {
                if (cmd == fcntl_cmds[i].flag) {
                        FMT_STRING(ctx, fcntl_cmds[i].name);

                        return 0;
                }
        }

        FMT_INT(ctx, cmd);

        ctx->extra = FCNTL_INVALID;
        return 0;
}

DEFINE_FMT(fcntl_arg3, int arg3)
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
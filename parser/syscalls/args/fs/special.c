#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/file.h>

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

DEFINE_FLAGS_ARRAY(flock_ops) = {
        INIT_FLAG_INFO(LOCK_SH),
        INIT_FLAG_INFO(LOCK_EX),
        INIT_FLAG_INFO(LOCK_UN),
        INIT_FLAG_INFO(LOCK_NB),
};

DEFINE_FLAGS_ARRAY(fadvise64_advices) = {
        INIT_FLAG_INFO(POSIX_FADV_NORMAL),
        INIT_FLAG_INFO(POSIX_FADV_SEQUENTIAL),
        INIT_FLAG_INFO(POSIX_FADV_RANDOM),
        INIT_FLAG_INFO(POSIX_FADV_NOREUSE),
        INIT_FLAG_INFO(POSIX_FADV_WILLNEED),
        INIT_FLAG_INFO(POSIX_FADV_DONTNEED),
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

DEFINE_FMT(flock_op, int op)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, op);

        FOR_EACH_FLAGS(flock_ops) {
                if (op == flock_ops[i].flag) {
                        FMT_STRING(ctx, flock_ops[i].name);
                        return 0;
                }
        }

        FMT_INT(ctx, op);
        return 0;
}

DEFINE_FMT(fadvise64_advice, int advice)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, advice);

        FOR_EACH_FLAGS(fadvise64_advices) {
                if (advice == fadvise64_advices[i].flag) {
                        FMT_STRING(ctx, fadvise64_advices[i].name);
                        return 0;
                }
        }

        FMT_INT(ctx, advice);
        return 0;
}
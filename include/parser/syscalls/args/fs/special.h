#pragma once

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"

#define FMT_FCNTL_CMD(ctx, cmd)   TRY_FMT(fmt_fcntl_cmd, ctx, cmd)
#define FMT_FCNTL_ARG3(ctx, arg3) TRY_FMT(fmt_fcntl_arg3, ctx, arg3)
#define FMT_FLOCK_OP(ctx, op)     TRY_FMT(fmt_flock_op, ctx, op)

/**
 * enum fcntl_fmt_type describes semantic interpretation of 3rd argument fcntl().
 * 
 * FCNTL_INT         - F_SETOWN, F_SETPIPE_SZ, F_SETSIG (FMT_INT)
 * FCNTL_FLAGS       - F_SETFL (FMT_OPEN_FLAGS)
 * FCNTL_FD_FLAGS    - F_SETFD (there is only one flag O_CLOEXEC, FMT_STRING)
 * FCNTL_FLOCK       - F_SETLK, F_SETLKW, F_GETLK (FMT_ADDR) TODO: make flock struct formatter.
 * FCNTL_INVALID     - Unknown fcntl command option.
 * FCNTL_IGNORE_ARG3 - Do not format 3rd argument, used by F_GET* commands (except F_GETLK).
 * 
 * Used in:
 * fmt_fcntl_cmd() for set ctx->extra to interpretation type of 3rd argument.
 * fmt_fcntl_arg3() for format specified type.
 */
enum fcntl_fmt_type {
        FCNTL_INT,
        FCNTL_FLAGS,
        FCNTL_FD_FLAGS,
        FCNTL_FLOCK,
        FCNTL_INVALID,
        FCNTL_IGNORE_ARG3
};

DEFINE_FMT(fcntl_cmd, int cmd);
DEFINE_FMT(fcntl_arg3, int arg3);
DEFINE_FMT(flock_op, int op);
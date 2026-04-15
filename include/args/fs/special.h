#pragma once

#include "args/helpers.h"
#include "args/flag_info.h"

#define FMT_FCNTL_ARG3(ctx, arg3)           TRY_FMT(fmt_fcntl_arg3, ctx, arg3)
#define FMT_FSCONFIG_VALUE(ctx, value, aux) TRY_FMT(fmt_fsconfig_value, ctx, value, aux)

/**
 * enum fcntl_fmt_type describes semantic interpretation of 3rd argument fcntl().
 * 
 * FCNTL_INT         - F_SETOWN, F_SETPIPE_SZ, F_SETSIG (FMT_INT)
 * FCNTL_FLAGS       - F_SETFL (FMT_OPEN_FLAGS)
 * FCNTL_FD_FLAGS    - F_SETFD (there is only one flag O_CLOEXEC, FMT_STRING)
 * FCNTL_FLOCK       - F_SETLK, F_SETLKW, F_GETLK (FMT_FLOCK_STRUCT).
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

int fmt_fcntl_arg3(struct parser_ctx_struct *ctx, void *arg3);
int fmt_fsconfig_value(struct parser_ctx_struct *ctx, const void *value, int aux);
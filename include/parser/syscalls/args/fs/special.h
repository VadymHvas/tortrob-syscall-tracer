#pragma once

#define FCNTL_IGNORE_ARG3 -1

#define FMT_FCNTL_CMD(ctx, cmd)   TRY_FMT(fmt_fcntl_cmd, ctx, cmd)
#define FMT_FCNTL_ARG3(ctx, arg3) TRY_FMT(fmt_fcntl_arg3, ctx, arg3)

enum fcntl_fmt_type {
        FCNTL_INT,
        FCNTL_FLAGS,
        FCNTL_FD_FLAGS,
        FCNTL_FLOCK,
        FCNTL_INVALID
};

#define FS_SPECIAL_FMTS \
        X(fcntl_cmd, int cmd); \
        X(fcntl_arg3, int arg3);

#define X(name, ...) DEFINE_SPECIAL_FMT(name, ##__VA_ARGS__)
FS_SPECIAL_FMTS
#undef X
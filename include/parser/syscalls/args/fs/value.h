#pragma once

#include "parser/syscalls/args/helpers.h"

#define FMT_FCNTL_CMD(ctx, cmd)           TRY_FMT(fmt_fcntl_cmd, ctx, cmd)
#define FMT_FLOCK_OP(ctx, op)             TRY_FMT(fmt_flock_op, ctx, op)
#define FMT_FADVISE64_ADVICE(ctx, advice) TRY_FMT(fmt_fadvise64_advice, ctx, advice)

int fmt_fcntl_cmd(struct parser_ctx_struct *ctx, int cmd);
int fmt_flock_op(struct parser_ctx_struct *ctx, int op);
int fmt_fadvise64_advice(struct parser_ctx_struct *ctx, int advice);
int fmt_statfs_fs_type(struct parser_ctx_struct *ctx, int fs_type);
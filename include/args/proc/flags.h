#pragma once

#include "args/helpers.h"

#define FMT_WAIT4_FLAGS(ctx, flags) TRY_FMT(fmt_wait4_flags, ctx, flags)
#define FMT_CLONE_FLAGS(ctx, flags) TRY_FMT(fmt_clone_flags, ctx, flags)

int fmt_wait4_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_clone_flags(struct parser_ctx_struct *ctx, unsigned long flags);
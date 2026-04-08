#pragma once

#include "parser/syscalls/args/helpers.h"

#define FMT_STATX_MASK(ctx, mask) TRY_FMT(fmt_statx_mask, ctx, mask)

int fmt_statx_mask(struct parser_ctx_struct *ctx, unsigned int mask);
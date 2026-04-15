#pragma once

#include "args/helpers.h"
#include "args/flag_info.h"

#define FMT_STATX_MASK(ctx, mask)     TRY_FMT(fmt_statx_mask, ctx, mask)
#define FMT_ACCESS_MASK(ctx, mask)    TRY_FMT(fmt_access_mask, ctx, mask)
#define FMT_FALLOCATE_MASK(ctx, mask) TRY_FMT(fmt_fallocate_mask, ctx, mask)
#define FMT_INOTIFY_MASK(ctx, mask)   TRY_FMT(fmt_inotify_mask, ctx, mask)

int fmt_statx_mask(struct parser_ctx_struct *ctx, unsigned int mask);
int fmt_access_mask(struct parser_ctx_struct *ctx, int mask);
int fmt_fallocate_mask(struct parser_ctx_struct *ctx, int mask);
int fmt_inotify_mask(struct parser_ctx_struct *ctx, unsigned int mask);
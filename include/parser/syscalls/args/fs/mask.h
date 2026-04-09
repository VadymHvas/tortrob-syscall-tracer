#pragma once

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"

#define FMT_STATX_MASK(ctx, mask)     TRY_FMT(fmt_statx_mask, ctx, mask)
#define FMT_ACCESS_MASK(ctx, mask)    TRY_FMT(fmt_access_mask, ctx, mask)
#define FMT_FALLOCATE_MASK(ctx, mask) TRY_FMT(fmt_fallocate_mask, ctx, mask)

DEFINE_MASK_FMT(statx);
DEFINE_MASK_FMT(access);
DEFINE_MASK_FMT(fallocate);
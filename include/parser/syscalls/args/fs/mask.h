#pragma once

#include "parser/syscalls/args/helpers.h"

#define FMT_STATX_MASK(ctx, mask) TRY_FMT(fmt_statx_mask, ctx, mask)
#define FMT_ACCESS_MASK(ctx, mask) TRY_FMT(fmt_access_mask, ctx, mask)

#define FS_MASK_FMTS \
        X(statx); \
        X(access);

#define X(name) DEFINE_MASK_FMT(name)
FS_MASK_FMTS
#undef X
#pragma once

#include "parser/syscalls/args/helpers.h"

#define FMT_OPEN_FLAGS(ctx, flags)      TRY_FMT(fmt_open_flags, ctx, flags)
#define FMT_AT_FLAGS(ctx, flags)        TRY_FMT(fmt_at_flags, ctx, flags)
#define FMT_RENAMEAT2_FLAGS(ctx, flags) TRY_FMT(fmt_renameat2_flags, ctx, flags)
#define FMT_DUP3_FLAGS(ctx, flags)      TRY_FMT(fmt_dup3_flags, ctx, flags)

struct parser_ctx_struct;

#define FS_FLAGS_FMTS \
        X(open); \
        X(at); \
        X(renameat2); \
        X(dup3);

#define X(name) DEFINE_FLAGS_FMT(name)
FS_FLAGS_FMTS
#undef X
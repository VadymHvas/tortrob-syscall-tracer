#pragma once

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"

#define FMT_OPEN_FLAGS(ctx, flags)      TRY_FMT(fmt_open_flags, ctx, flags)
#define FMT_AT_FLAGS(ctx, flags)        TRY_FMT(fmt_at_flags, ctx, flags)
#define FMT_RENAMEAT2_FLAGS(ctx, flags) TRY_FMT(fmt_renameat2_flags, ctx, flags)
#define FMT_DUP3_FLAGS(ctx, flags)      TRY_FMT(fmt_dup3_flags, ctx, flags)
#define FMT_RWF_FLAGS(ctx, flags)       TRY_FMT(fmt_rwf_flags, ctx, flags)

struct parser_ctx_struct;

DEFINE_FLAGS_FMT(open);
DEFINE_FLAGS_FMT(at);
DEFINE_FLAGS_FMT(renameat2);
DEFINE_FLAGS_FMT(dup3);
DEFINE_FLAGS_FMT(rwf);
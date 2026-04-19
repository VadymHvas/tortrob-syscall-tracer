#pragma once

#include <sys/resource.h>

#include "args/helpers.h"
#include "args/struct_info.h"

#define FMT_RUSAGE_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_rusage_struct, addr, sizeof(struct rusage))

int fmt_rusage_struct(struct parser_ctx_struct *ctx, struct rusage *rusage);
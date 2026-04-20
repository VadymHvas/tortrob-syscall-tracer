#pragma once

#include "args/helpers.h"

#define FMT_WAIT4_STATUS(ctx, addr) TRY_FMT(fmt_wait4_status, ctx, addr)

int fmt_wait4_status(struct parser_ctx_struct *ctx, unsigned long long addr);
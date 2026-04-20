#pragma once

#include <sys/wait.h>

#include "args/helpers.h"

#define FMT_WAIT4_STATUS(ctx, addr) TRY_FMT(fmt_wait4_status, ctx, addr)
#define FMT_IDTYPE(ctx, idtype)     TRY_FMT(fmt_idtype, ctx, idtype)
#define FMT_SIGNAL_NAME(ctx, sig)   TRY_FMT(fmt_signal_name, ctx, sig)

int fmt_wait4_status(struct parser_ctx_struct *ctx, unsigned long long addr);
int fmt_idtype(struct parser_ctx_struct *ctx, idtype_t idtype);
int fmt_signal_name(struct parser_ctx_struct *ctx, int sig);
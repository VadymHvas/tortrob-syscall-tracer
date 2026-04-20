#pragma once

#include <signal.h>
#include <sys/resource.h>

#include "args/helpers.h"
#include "args/struct_info.h"

#include "args/proc/value.h"

#define INIT_FIELD_INFO_SIG(struct_type, field) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field, fmt_signal_name)

#define FMT_RUSAGE_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_rusage_struct, addr, sizeof(struct rusage))

#define FMT_SIGINFO_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_siginfo_struct, addr, sizeof(siginfo_t))

int fmt_rusage_struct(struct parser_ctx_struct *ctx, struct rusage *rusage);
int fmt_siginfo_struct(struct parser_ctx_struct *ctx, siginfo_t *siginfo);
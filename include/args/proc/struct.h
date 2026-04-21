#pragma once

#include <signal.h>
#include <linux/sched.h>
#include <sys/resource.h>

#include "args/helpers.h"
#include "args/struct_info.h"

#include "args/proc/value.h"
#include "args/proc/flags.h"
#include "args/proc/mask.h"

#define INIT_FIELD_INFO_SIG(struct_type, field) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field, fmt_sig_name)

#define INIT_FIELD_INFO_CLONE_FLAGS(struct_type, field) \
        INIT_FIELD_INFO(TYPE_ULONG, REPR_FLAGS, struct_type, field, fmt_clone_flags)

#define INIT_FIELD_INFO_SIGSET(struct_type, field) \
        INIT_FIELD_INFO(TYPE_PTR, REPR_FLAGS, struct_type, field, fmt_signal_set)

#define INIT_FIELD_INFO_SA_FLAGS(struct_type, field) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field, fmt_sigaction_flags)

#define FMT_RUSAGE_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_rusage_struct, addr, sizeof(struct rusage))

#define FMT_SIGINFO_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_siginfo_struct, addr, sizeof(siginfo_t))

#define FMT_CLONE_ARGS_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_clone_args_struct, addr, sizeof(struct clone_args))

#define FMT_SIGACTION_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_sigaction_struct, addr, sizeof(struct sigaction))

int fmt_rusage_struct(struct parser_ctx_struct *ctx, struct rusage *rusage);
int fmt_siginfo_struct(struct parser_ctx_struct *ctx, siginfo_t *siginfo);
int fmt_clone_args_struct(struct parser_ctx_struct *ctx, struct clone_args *cl_args);
int fmt_sigaction_struct(struct parser_ctx_struct *ctx, struct sigaction *act);
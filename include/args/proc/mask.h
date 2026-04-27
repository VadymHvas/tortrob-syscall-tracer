#pragma once

#include <signal.h>

#include "core/trace.h"

#define FMT_SIGSET(ctx, addr) \
        do { \
                sigset_t sigset; \
                if (read_tracee_mem(ctx->tracee, addr, &sigset, sizeof(sigset)) == -1) { \
                        FMT_ADDR(ctx, addr); \
                } else { \
                        fmt_signal_set(ctx, &sigset); \
                } \
        } while (0)

int fmt_signal_set(struct parser_ctx_struct *ctx, sigset_t *sigset);
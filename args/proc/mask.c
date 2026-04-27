#define _GNU_SOURCE

#include <signal.h>

#include "args/flag_info.h"
#include "args/helpers.h"

#include "args/proc/mask.h"
#include "args/proc/value.h"

#include "core/trace.h"

int fmt_signal_set(struct parser_ctx_struct *ctx, sigset_t *sigset)
{
        FMT_STRING(ctx, "[");

        int first = 1;

        for (int i = 1; i < NSIG; i++) {
                int res = sigismember(sigset, i);
                if (res == 1) {
                        if (!first)
                                FMT_STRING(ctx, " ");

                        FMT_SIGNAL_NAME(ctx, i);
                        first = 0;
                }
        }

        FMT_STRING(ctx, "]");
        return 0;
}
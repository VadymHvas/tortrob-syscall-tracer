#define _GNU_SOURCE

#include <sys/wait.h>

#include "args/proc/flags.h"
#include "args/flag_info.h"
#include "args/helpers.h"

DEFINE_FLAGS_ARRAY(wait4_flags) = {
        INIT_FLAG_INFO(WNOHANG),
        INIT_FLAG_INFO(WUNTRACED),
        INIT_FLAG_INFO(WCONTINUED),
        INIT_FLAG_INFO(WNOWAIT),
        INIT_FLAG_INFO(WNOWAIT),
        INIT_FLAG_INFO(WEXITED)
};

int fmt_wait4_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(ctx, flags, wait4_flags, FLAGS_ARR_SIZE(wait4_flags), 1); 
}
#define _GNU_SOURCE

#include <sys/wait.h>
#include <sched.h>

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

DEFINE_FLAGS_ARRAY(clone_flags) = {
        INIT_FLAG_INFO(CLONE_VM),
        INIT_FLAG_INFO(CLONE_FS),
        INIT_FLAG_INFO(CLONE_FILES),
        INIT_FLAG_INFO(CLONE_SIGHAND),
        INIT_FLAG_INFO(CLONE_PIDFD),
        INIT_FLAG_INFO(CLONE_PTRACE),
        INIT_FLAG_INFO(CLONE_VFORK),
        INIT_FLAG_INFO(CLONE_PARENT),
        INIT_FLAG_INFO(CLONE_THREAD),
        INIT_FLAG_INFO(CLONE_NEWNS),
        INIT_FLAG_INFO(CLONE_SYSVSEM),
        INIT_FLAG_INFO(CLONE_SETTLS),
        INIT_FLAG_INFO(CLONE_PARENT_SETTID),
        INIT_FLAG_INFO(CLONE_CHILD_CLEARTID),
        INIT_FLAG_INFO(CLONE_DETACHED),
        INIT_FLAG_INFO(CLONE_UNTRACED),
        INIT_FLAG_INFO(CLONE_CHILD_SETTID),
};

DEFINE_FLAGS_ARRAY(sigaction_flags) = {
        INIT_FLAG_INFO(SA_NOCLDSTOP),
        INIT_FLAG_INFO(SA_NOCLDWAIT),
        INIT_FLAG_INFO(SA_SIGINFO),
        INIT_FLAG_INFO(SA_ONSTACK),
        INIT_FLAG_INFO(SA_RESTART),
        INIT_FLAG_INFO(SA_NODEFER),
        INIT_FLAG_INFO(SA_RESETHAND),
        INIT_FLAG_INFO(SA_NOMASK),
        INIT_FLAG_INFO(SA_ONESHOT),
        INIT_FLAG_INFO(SA_INTERRUPT)
};

int fmt_wait4_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(ctx, flags, wait4_flags, FLAGS_ARR_SIZE(wait4_flags), 1); 
}

int fmt_clone_flags(struct parser_ctx_struct *ctx, unsigned long flags)
{
        return fmt_flags_generic(ctx, flags, clone_flags, FLAGS_ARR_SIZE(clone_flags), 1); 
}

int fmt_sigaction_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(ctx, flags, sigaction_flags, FLAGS_ARR_SIZE(sigaction_flags), 1); 
}
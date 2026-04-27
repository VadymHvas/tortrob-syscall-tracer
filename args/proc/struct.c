#define _GNU_SOURCE

#include <signal.h>
#include <linux/sched.h>
#include <sys/resource.h>

#include "args/proc/struct.h"
#include "args/struct_info.h"
#include "args/helpers.h"

DEFINE_FIELDS_ARRAY(rusage_fields) = {
        INIT_FIELD_INFO_LONG(struct rusage, ru_utime),
        INIT_FIELD_INFO_LONG(struct rusage, ru_stime),
        INIT_FIELD_INFO_LONG(struct rusage, ru_maxrss),
        INIT_FIELD_INFO_LONG(struct rusage, ru_ixrss),
        INIT_FIELD_INFO_LONG(struct rusage, ru_idrss),
        INIT_FIELD_INFO_LONG(struct rusage, ru_isrss),
        INIT_FIELD_INFO_LONG(struct rusage, ru_minflt),
        INIT_FIELD_INFO_LONG(struct rusage, ru_majflt),
        INIT_FIELD_INFO_LONG(struct rusage, ru_nswap),
        INIT_FIELD_INFO_LONG(struct rusage, ru_inblock),
        INIT_FIELD_INFO_LONG(struct rusage, ru_oublock),
        INIT_FIELD_INFO_LONG(struct rusage, ru_msgsnd),
        INIT_FIELD_INFO_LONG(struct rusage, ru_msgrcv),
        INIT_FIELD_INFO_LONG(struct rusage, ru_nsignals),
        INIT_FIELD_INFO_LONG(struct rusage, ru_nvcsw),
        INIT_FIELD_INFO_LONG(struct rusage, ru_nivcsw)
};

DEFINE_FIELDS_ARRAY(siginfo_fields) = {
        INIT_FIELD_INFO_SIG(siginfo_t, si_signo),
        INIT_FIELD_INFO_INT(siginfo_t, si_errno),
        INIT_FIELD_INFO_INT(siginfo_t, si_code)
};

DEFINE_FIELDS_ARRAY(clone_args_fields) = {
        INIT_FIELD_INFO_CLONE_FLAGS(struct clone_args, flags),
        INIT_FIELD_INFO_PTR(struct clone_args, pidfd),
        INIT_FIELD_INFO_PTR(struct clone_args, child_tid),
        INIT_FIELD_INFO_PTR(struct clone_args, parent_tid),
        INIT_FIELD_INFO_SIG(struct clone_args, exit_signal),
        INIT_FIELD_INFO_PTR(struct clone_args, stack),
        INIT_FIELD_INFO_ULONG(struct clone_args, stack_size),
        INIT_FIELD_INFO_PTR(struct clone_args, tls)
};

DEFINE_FIELDS_ARRAY(sigaction_fields) = {
        INIT_FIELD_INFO_PTR(struct sigaction, sa_handler),
        INIT_FIELD_INFO_SIGSET(struct sigaction, sa_mask),
        INIT_FIELD_INFO_SA_FLAGS(struct sigaction, sa_flags),
};

int fmt_rusage_struct(struct parser_ctx_struct *ctx, struct rusage *rusage)
{
        return fmt_struct_generic(ctx, rusage, rusage_fields, FIELDS_ARR_SIZE(rusage_fields));
}

int fmt_siginfo_struct(struct parser_ctx_struct *ctx, siginfo_t *siginfo)
{
        return fmt_struct_generic(ctx, siginfo, siginfo_fields, FIELDS_ARR_SIZE(siginfo_fields));
}

int fmt_clone_args_struct(struct parser_ctx_struct *ctx, struct clone_args *cl_args)
{
        return fmt_struct_generic(ctx, cl_args, clone_args_fields, FIELDS_ARR_SIZE(clone_args_fields));
}

int fmt_sigaction_struct(struct parser_ctx_struct *ctx, struct sigaction *act)
{
        return fmt_struct_generic(ctx, act, sigaction_fields, FIELDS_ARR_SIZE(sigaction_fields));
}
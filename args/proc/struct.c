#define _GNU_SOURCE

#include <signal.h>
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

int fmt_rusage_struct(struct parser_ctx_struct *ctx, struct rusage *rusage)
{
        return fmt_struct_generic(ctx, rusage, rusage_fields, FIELDS_ARR_SIZE(rusage_fields));
}

int fmt_siginfo_struct(struct parser_ctx_struct *ctx, siginfo_t *siginfo)
{
        return fmt_struct_generic(ctx, siginfo, siginfo_fields, FIELDS_ARR_SIZE(siginfo_fields));
}
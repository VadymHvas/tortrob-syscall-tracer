#define _GNU_SOURCE

#include <signal.h>
#include <sys/wait.h>

#include "args/proc/value.h"
#include "args/value_info.h"
#include "args/helpers.h"

#include "core/trace.h"

DEFINE_VALUES_ARRAY(idtypes) = {
        INIT_VALUE(P_ALL),
        INIT_VALUE(P_PID),
        INIT_VALUE(P_PGID)
};

int fmt_wait4_status(struct parser_ctx_struct *ctx, unsigned long long addr)
{
        FMT_STRING(ctx, "[");

        long status;

        if (read_tracee_word(ctx->tracee, (void *)addr, &status) == -1)
                return fmt_string(ctx, "<failed>");

        if (WIFEXITED(status)) {
                FMT_STRING(ctx, "WIFEXITED(s) == ");
                FMT_INT(ctx, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
                FMT_STRING(ctx, "killed by signal ");
                FMT_SIGNAL_NAME(ctx, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
                FMT_STRING(ctx, "WIFSTOPPED(s) == ");
                FMT_INT(ctx, WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
                FMT_STRING(ctx, "WIFCONTINUED(s)");
        } else {
                FMT_STRING(ctx, "<unknown>");
        }

        FMT_STRING(ctx, "]");

        return 0;
}

int fmt_idtype(struct parser_ctx_struct *ctx, idtype_t idtype)
{
        return fmt_value_generic(ctx, idtype, idtypes, VALUES_ARR_SIZE(idtypes));
}

int fmt_sig_name(struct parser_ctx_struct *ctx, int sig)
{
        if (sig >= SIGRTMIN && sig <= SIGRTMAX) {
                FMT_STRING(ctx, "SIGRTMIN+");
                return fmt_int(ctx, sig - SIGRTMIN);
        }

        switch (sig) {
        case SIGHUP: 
                FMT_STRING(ctx, "SIGHUP");
                break;
        case SIGINT: 
                FMT_STRING(ctx, "SIGINT");
                break;
        case SIGQUIT: 
                FMT_STRING(ctx, "SIGQUIT");
                break;
        case SIGILL: 
                FMT_STRING(ctx, "SIGILL");
                break;
        case SIGTRAP: 
                FMT_STRING(ctx, "SIGTRAP");
                break;
        case SIGABRT: 
                FMT_STRING(ctx, "SIGABRT");
                break;
        case SIGBUS: 
                FMT_STRING(ctx, "SIGBUS");
                break;
        case SIGFPE: 
                FMT_STRING(ctx, "SIGFPE");
                break;
        case SIGKILL: 
                FMT_STRING(ctx, "SIGKILL");
                break;
        case SIGUSR1: 
                FMT_STRING(ctx, "SIGUSR1");
                break;
        case SIGSEGV: 
                FMT_STRING(ctx, "SIGSEGV");
                break;
        case SIGUSR2: 
                FMT_STRING(ctx, "SIGUSR2");
                break;
        case SIGPIPE: 
                FMT_STRING(ctx, "SIGPIPE");
                break;
        case SIGALRM: 
                FMT_STRING(ctx, "SIGALRM");
                break;
        case SIGTERM: 
                FMT_STRING(ctx, "SIGTERM");
                break;
        case SIGSTKFLT: 
                FMT_STRING(ctx, "SIGSTKFLT");
                break;
        case SIGCHLD: 
                FMT_STRING(ctx, "SIGCHLD");
                break;
        case SIGCONT: 
                FMT_STRING(ctx, "SIGCONT");
                break;
        case SIGSTOP: 
                FMT_STRING(ctx, "SIGSTOP");
                break;
        case SIGTSTP: 
                FMT_STRING(ctx, "SIGTSTP");
                break;
        case SIGTTIN: 
                FMT_STRING(ctx, "SIGTTIN");
                break;
        case SIGTTOU: 
                FMT_STRING(ctx, "SIGTTOU");
                break;
        case SIGURG: 
                FMT_STRING(ctx, "SIGURG");
                break;
        case SIGXCPU: 
                FMT_STRING(ctx, "SIGXCPU");
                break;
        case SIGXFSZ: 
                FMT_STRING(ctx, "SIGXFSZ");
                break;
        case SIGVTALRM: 
                FMT_STRING(ctx, "SIGVTALRM");
                break;
        case SIGPROF: 
                FMT_STRING(ctx, "SIGPROF");
                break;
        case SIGWINCH: 
                FMT_STRING(ctx, "SIGWINCH");
                break;
        case SIGIO: 
                FMT_STRING(ctx, "SIGIO");
                break;
        case SIGPWR: 
                FMT_STRING(ctx, "SIGPWR");
                break;
        case SIGSYS: 
                FMT_STRING(ctx, "SIGSYS");
                break;
        default: 
                FMT_STRING(ctx, "UNKNOWN(");
                FMT_INT(ctx, sig);
                FMT_STRING(ctx, ")");
                break;
        }
}
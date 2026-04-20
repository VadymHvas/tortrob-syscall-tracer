#include <signal.h>
#include <sys/wait.h>

#include "args/proc/value.h"
#include "args/value_info.h"
#include "args/helpers.h"

#include "core/trace.h"

static const char *siglist[] = {
        [0] = "UNKNOWN",

        [SIGHUP]    = "SIGHUP",
        [SIGINT]    = "SIGINT",
        [SIGQUIT]   = "SIGQUIT",
        [SIGILL]    = "SIGILL",
        [SIGTRAP]   = "SIGTRAP",
        [SIGABRT]   = "SIGABRT",
        [SIGBUS]    = "SIGBUS",
        [SIGFPE]    = "SIGFPE",
        [SIGKILL]   = "SIGKILL",
        [SIGUSR1]   = "SIGUSR1",
        [SIGSEGV]   = "SIGSEGV",
        [SIGUSR2]   = "SIGUSR2",
        [SIGPIPE]   = "SIGPIPE",
        [SIGALRM]   = "SIGALRM",
        [SIGTERM]   = "SIGTERM",
        [SIGSTKFLT] = "SIGSTKFLT",
        [SIGCHLD]   = "SIGCHLD",
        [SIGCONT]   = "SIGCONT",
        [SIGSTOP]   = "SIGSTOP",
        [SIGTSTP]   = "SIGTSTP",
        [SIGTTIN]   = "SIGTTIN",
        [SIGTTOU]   = "SIGTTOU",
        [SIGURG]    = "SIGURG",
        [SIGXCPU]   = "SIGXCPU",
        [SIGXFSZ]   = "SIGXFSZ",
        [SIGVTALRM] = "SIGVTALRM",
        [SIGPROF]   = "SIGPROF",
        [SIGWINCH]  = "SIGWINCH",
        [SIGIO]     = "SIGIO",
        [SIGPWR]    = "SIGPWR",
        [SIGSYS]    = "SIGSYS"
};

static const char *get_sig_name(int sig);

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
                FMT_STRING(ctx, get_sig_name(WTERMSIG(status)));
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

static const char *get_sig_name(int sig)
{
        if (sig <= 0 || sig >= (int)(sizeof(siglist)/sizeof(siglist[0])))
                return "UNKNOWN";

        return siglist[sig] ? siglist[sig] : "UNKNOWN";
}
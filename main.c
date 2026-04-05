#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>

#include "parser/opt.h"
#include "core/trace.h"

int main(int argc, char **argv)
{
        if (argc < 2) {
                fprintf(stderr, "tortrob-strace: specify the target program\n");
                return 1;
        }

        struct trace_opts *opts = parse_opts(argc, argv);
        pid_t *child;

        if (!opts)
                return 1;

        if (init_trace(argv, opts, child) == -1)
                goto err;

        trace_loop(*child);

        putc('\n', stdout);
        cleanup_trace(opts, *child);
        return 0;
err:
        free(opts);
        return 1;
}
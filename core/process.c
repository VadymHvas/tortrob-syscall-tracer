/*
 * core/process.c - Tracer process manager.
 *
 * This module implements methods for 
 * working with processes by the tracer.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>

#include "core/process.h"
#include "parser/opt.h"

/* Main spawn process function */
int __do_spawn(char **argv, pid_t *tracee)
{
        *tracee = fork();
        if (*tracee == -1) {
                perror("tortrob-strace");
                return -1;
        }

        if (*tracee == 0) {
                ptrace(PTRACE_TRACEME, 0, NULL, NULL);
                execvp(argv[optind], &argv[optind]);
                perror(argv[optind]);
                exit(1);
        }

        return 0;
}

/* Main process attaching function */
static int __do_attach(char **argv, pid_t *tracee)
{
        if (ptrace(PTRACE_ATTACH, *tracee, NULL, NULL) == -1) {
                perror("tortrob-strace");
                return -1;
        }

        return 0;
}

static pid_t __get_pid(struct trace_opts *opts)
{
        return (pid_t)atoi(opts->pid);
}

int __attach_tracee(char **argv, struct trace_opts *opts, pid_t *tracee)
{
        *tracee = __get_pid(opts);
        if (__do_attach(argv, tracee) == -1)
                return -1;        

        printf("Attached to process %d\n", *tracee);
        return 0;
}
/*
 * core/trace.c - Core of tracing logic.
 * 
 * This module is responsible for initializing the tracer (spawn/attach)
 * and implements the tracer main loop.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>

#include "core/trace.h"
#include "core/process.h"
#include "core/dispatch.h"
#include "parser/syscall.h"
#include "parser/opt.h"

static int __wait_and_set_tracesysgood(pid_t tracee);
static int __trace_syscall_and_wait(pid_t tracee, int *status);

int init_trace(char **argv, struct trace_opts *opts, pid_t *tracee)
{
        if (opts->attach) {
                if (attach_tracee(argv, opts, tracee) == -1)
                        return -1;
        } else {
                if (do_spawn(argv, tracee) == -1)
                        return -1;
        }

        return __wait_and_set_tracesysgood(*tracee);
}

void trace_loop(pid_t tracee)
{
        int status;

        while (1) {
                if (__trace_syscall_and_wait(tracee, &status))
                        break;
                
                /* 0x80 bit is set by __wait_and_set_tracesysgood(). */
                if (WIFSTOPPED(status) && (WSTOPSIG(status) & 0x80)) {
                        struct user_regs_struct regs;
                        ptrace(PTRACE_GETREGS, tracee, NULL, &regs);
                        entry_or_exit_syscall(&regs);
                }
        }
}

void cleanup_trace(struct trace_opts *opts, pid_t tracee)
{
        if (opts->attach && tracee > 0)
                ptrace(PTRACE_DETACH, tracee, NULL, NULL);

        free(opts);
}

static int __wait_and_set_tracesysgood(pid_t tracee)
{
        int status;
        waitpid(tracee, &status, 0);

        if (!WIFSTOPPED(status))
                return -1;

        /* PTRACE_O_TRACESYSGOOD option adds 0x80 bit to the SIGTRAP. */
        ptrace(PTRACE_SETOPTIONS, tracee, NULL, PTRACE_O_TRACESYSGOOD);
        return 0;
}

static int __trace_syscall_and_wait(pid_t tracee, int *status)
{
        ptrace(PTRACE_SYSCALL, tracee, NULL, NULL);
        waitpid(tracee, status, 0);

        if (WIFEXITED(*status)) 
                return 1;
        
        return 0;
}
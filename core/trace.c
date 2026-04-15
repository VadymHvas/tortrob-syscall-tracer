/*
 * core/trace.c - Core of tracing logic.
 * 
 * This module is responsible for initializing the tracer (spawn/attach)
 * and implements the tracer main loop.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>
#include <errno.h>

#include "core/trace.h"
#include "core/process.h"
#include "core/dispatch.h"
#include "parser/syscall.h"
#include "parser/opt.h"
#include "parser/syscalls/args/helpers.h"

#define SYSCALL_BUF_SIZE 256

static int wait_and_set_tracesysgood(pid_t tracee);
static int trace_syscall_and_wait(pid_t tracee, int *status);

int init_trace(char **argv, struct trace_opts *opts, pid_t *tracee)
{
        if (opts->attach) {
                if (attach_tracee(argv, opts, tracee) == -1)
                        return -1;
        } else {
                if (do_spawn(argv, tracee) == -1)
                        return -1;
        }

        return wait_and_set_tracesysgood(*tracee);
}

void trace_loop(pid_t tracee)
{
        int status;
        char output_buf[SYSCALL_BUF_SIZE];
        INIT_PARSER_CTX(ctx, output_buf, SYSCALL_BUF_SIZE, tracee);

        while (1) {
                if (trace_syscall_and_wait(tracee, &status))
                        break;
                
                /* 0x80 bit is set by wait_and_set_tracesysgood(). */
                if (WIFSTOPPED(status) && (WSTOPSIG(status) & 0x80)) {
                        struct user_regs_struct regs;
                        ptrace(PTRACE_GETREGS, tracee, NULL, &regs);
                        enter_or_exit_syscall(&ctx, &regs);
                }
        }
}

void cleanup_trace(struct trace_opts *opts, pid_t tracee)
{
        if (opts->attach && tracee > 0)
                ptrace(PTRACE_DETACH, tracee, NULL, NULL);

        free(opts);
}

size_t read_tracee_mem(pid_t tracee, const void *addr, void *buf, size_t size)
{
        size_t word_size = sizeof(long);
        uintptr_t start = (uintptr_t)addr;

        uintptr_t aligned = start & ~(word_size - 1);
        size_t offset = start - aligned;

        size_t copied = 0;

        while (copied < size) {
                errno = 0;
                long data = ptrace(PTRACE_PEEKDATA, tracee, (void *)(aligned), NULL);

                if (data == -1 && errno)
                        return -1;

                size_t copy_from = offset;
                size_t copy_size = word_size - offset;

                if (copy_size > size - copied)
                        copy_size = size - copied;

                memcpy((char *)buf + copied, ((char *)&data) + copy_from, copy_size);

                copied += copy_size;
                aligned += word_size;
                offset = 0;
        }

    return copied;
}

int read_tracee_word(pid_t tracee, const void *addr, long *out)
{
        errno = 0;
        long data = ptrace(PTRACE_PEEKDATA, tracee, addr, NULL);

        if (data == -1 && errno)
                return -1;

        if (out)
                *out = data;
        
        return 0;
}

static int wait_and_set_tracesysgood(pid_t tracee)
{
        int status;
        waitpid(tracee, &status, 0);

        if (!WIFSTOPPED(status))
                return -1;

        /* PTRACE_O_TRACESYSGOOD option adds 0x80 bit to the SIGTRAP. */
        ptrace(PTRACE_SETOPTIONS, tracee, NULL, PTRACE_O_TRACESYSGOOD);
        return 0;
}

static int trace_syscall_and_wait(pid_t tracee, int *status)
{
        ptrace(PTRACE_SYSCALL, tracee, NULL, NULL);
        waitpid(tracee, status, 0);

        if (WIFEXITED(*status)) 
                return 1;
        
        return 0;
}
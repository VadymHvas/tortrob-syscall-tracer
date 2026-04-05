#pragma once

#include <sys/types.h>

#include "parser/opt.h"

/**
 * do_spawn - Low-level create a new child process and prepare it for tracing.
 * @argv:   Argument vector for the target program (argv[optind] is the binary).
 * @tracee: Pointer to store the PID of the newly created child.
 *
 * This function:
 * 1. Forks a new process.
 * 2. In the child: calls PTRACE_TRACEME and then execvp().
 * 3. In the parent: returns the child's PID via @tracee.
 *
 * Return: 0 on success, -1 if fork() or execvp() fails.
 */
int do_spawn(char **argv, pid_t *tracee);

/**
 * attach_tracee - Attach to an already running process.
 * @argv:   Command line arguments (currently unused in this mode).
 * @opts:   Options containing the target PID string.
 * @tracee: Pointer to store the PID of the attached process.
 *
 * Uses PTRACE_ATTACH to seize control of an existing process. 
 * Note that the caller must wait for the process to stop before 
 * issuing further ptrace commands.
 *
 * Return: 0 on success, -1 if the PID is invalid or attach fails.
 */
int attach_tracee(char **argv, struct trace_opts *opts, pid_t *tracee);
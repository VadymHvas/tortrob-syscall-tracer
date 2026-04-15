#pragma once

#include <sys/types.h>

#include "parser/opt.h"

/**
 * init_trace - Initialize the tracing session.
 * @argv:   Command line arguments (for spawning a new process).
 * @opts:   Parsed options (determines if we attach or spawn).
 * @tracee: Pointer to store the resulting PID of the traced process.
 *
 * This function handles the setup phase:
 * 1. Either forks and execs a new process or attaches to an existing PID.
 * 2. Waits for the child to stop.
 * 3. Sets PTRACE_O_TRACESYSGOOD to enable advanced syscall tracking.
 *
 * Return: 0 on success, -1 on fatal error (e.g., fork or attach failure).
 */
int init_trace(char **argv, struct trace_opts *opts, pid_t *tracee);

/**
 * trace_loop - The main execution engine of the tracer.
 * @tracee: The PID of the process being traced.
 *
 * This is a blocking function that runs until the tracee exits.
 * It uses PTRACE_SYSCALL to intercept every system call entry and exit,
 * delegating the processing logic to the interceptor module.
 */
void trace_loop(pid_t tracee);

/**
 * cleanup_trace - Finalize and release tracing resources.
 * @opts:   Pointer to the options structure (to check if we need to detach).
 * @tracee: The PID of the process to clean up.
 *
 * Performs necessary cleanup: detaches from the process if it was attached
 * with -p, and frees allocated memory for options.
 */
void cleanup_trace(struct trace_opts *opts, pid_t tracee);

/**
 * read_tracee_mem - Reads the memory of the tracee at address addr.
 * 
 * @tracee: Tracee process PID.
 * @addr:   Memory address (src).
 * @buf:    Buffer address (dest).
 * @size:   Bytes to read.
 * 
 * Return: Number of bytes successfully read, or -1 on error.
 */
size_t read_tracee_mem(pid_t tracee, const void *addr, void *buf, size_t size);

/**
 * read_tracee_word - Reads a word from the tracee's memory at address addr.
 * 
 * @tracee: Tracee process PID.
 * @addr:   Memory address (src).
 * @out:    Pointer to store the read value.
 *
 * Return: 0 on success, -1 on error.
 */
int read_tracee_word(pid_t tracee, const void *addr, long *out);
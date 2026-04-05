#pragma once

/**
 * struct trace_opts - Tracing options structure.
 * 
 * @attach: Boolean flag; if non-zero, the tracer attaches to an existing process.
 * @pid:    String representation of the target PID.
 * 
 * This structure holds the parsed command-line flags that determine
 * whether we spawn a new child or hook into a running PID.
 */
struct trace_opts {
        int attach; /* 1 if -p <pid> is used, 0 for spawning a new process */
        char *pid;  /* The PID to attach to (as a string from argv) */
};

struct trace_opts *parse_opts(int argc, char **argv);
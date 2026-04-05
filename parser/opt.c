/*
 * parser/opt.c - Options parser and manager.
 */

#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser/opt.h"

static struct trace_opts *__init_trace_opts(struct trace_opts *opts)
{
        opts->attach = 0;
        opts->pid = NULL;

        return opts;
}

static struct trace_opts *__alloc_trace_opts()
{
        struct trace_opts *opts = (struct trace_opts *)malloc(sizeof(struct trace_opts));

        if (!opts)
                return NULL;

        return __init_trace_opts(opts);
}

struct trace_opts *parse_opts(int argc, char **argv)
{
        int opt;
        struct trace_opts *opts = __alloc_trace_opts();

        if (!opts)
                return NULL;

        while ((opt = getopt(argc, argv, ":p:")) != -1) {
                switch (opt)
                {
                case 't':
                        opts->attach = 1;
                        opts->pid = optarg;
                        break;

                case ':':
                        fprintf(stderr, "tortrob-strace: option -%c requires an argument [PID]\n", optopt);
                        free(opts);
                        return NULL;

                case '?':
                        fprintf(stderr, "tortrob-strace: unknown option -%c\n", optopt);
                        free(opts);
                        return NULL;
                        
                default:
                        break;
                }
        }

        return opts;
}
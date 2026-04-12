/**
 * parser/syscalls/parser.c - Core syscall parser lookup layer.
 *
 * Responsible for dispatching syscalls to their corresponding
 * argument parsing functions based on syscall number.
 * Falls back to a default parser when no specific implementation exists.
 */

#include <stdio.h>
#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/fs.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/args/helpers.h"

#define INIT_PARSER_GROUP(g) { g, sizeof(g) / sizeof(g[0]) },

/* 
 * Array of syscall parser groups.
 * Each group contains a set of syscalls belonging to the same category
 * (e.g., filesystem, process, network) along with their argument parsers.
 */
static const struct parser_group_struct syscall_parsers[] = {
        INIT_PARSER_GROUP(fs_syscalls)
};

static int default_syscall_parser(struct parser_ctx_struct *ctx, int nr_args, reg_t args[]);
static int handle_syscall_parser(struct parser_ctx_struct *ctx, struct parser_struct *entry, reg_t args[]);

int syscall_parse(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, reg_t args[])
{
        /*
        * Perform a linear search across all parser groups to find
        * a matching syscall number and dispatch to its parser.
        *
        * NOTE: This is O(N) and may be optimized in the future.
        */
        for (int i = 0; i < sizeof(syscall_parsers) / sizeof(syscall_parsers[0]); i++) {
                for (int j = 0; j < syscall_parsers[i].len; j++) {
                        if (syscall->nr == syscall_parsers[i].entry[j].nr) {
                                return handle_syscall_parser(ctx, &syscall_parsers[i].entry[j], args);
                        }
                }
        }

        return default_syscall_parser(ctx, syscall->args, args);
}

static int default_syscall_parser(struct parser_ctx_struct *ctx, int nr_args, reg_t args[])
{
        if (!ctx->in_syscall) {
                for (int i = 0; i < nr_args; i++) {
                        if (i != 0)
                                FMT_SEPARATOR(ctx);
                        FMT_LLU(ctx, args[i]);
                }
        }

        return 0;
}

static int handle_syscall_parser(struct parser_ctx_struct *ctx, struct parser_struct *entry, reg_t args[])
{
        if (!ctx->in_syscall) {
                if (entry->needs_enter)
                        return entry->enter_parse(ctx, args);
                
                return 0;
        }

        if (entry->needs_exit)
                return entry->exit_parse(ctx, args);

        return 0;
}
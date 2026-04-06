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
#include "parser/syscalls/helpers.h"

#define INIT_PARSER_GROUP(g) { g, sizeof(g) / sizeof(g[0]) },

/* 
 * Array of syscall parser groups.
 * Each group contains a set of syscalls belonging to the same category
 * (e.g., filesystem, process, network) along with their argument parsers.
 */
static const struct parser_group_struct syscall_parsers[] = {
        INIT_PARSER_GROUP(fs_syscalls)
};

static int default_syscall_parser(char *buf, size_t bufsize, int nr_args,
        size_t *offset, raw_reg args[]);

int syscall_parse(char *buf, size_t bufsize, 
        const struct syscall_entry *syscall, size_t *offset, raw_reg args[])
{
        /*
        * Perform a linear search across all parser groups to find
        * a matching syscall number and dispatch to its parser.
        *
        * NOTE: This is O(N) and may be optimized in the futureю
        */
        for (int i = 0; i < sizeof(syscall_parsers) / sizeof(syscall_parsers[0]); i++) {
                for (int j = 0; j < syscall_parsers[i].len; j++) {
                        if (syscall->nr == syscall_parsers[i].entry[j].nr) {
                                INIT_PARSER_CTX(ctx, buf, bufsize, offset);

                                return syscall_parsers[i].entry[j].parse_func(&ctx, args);
                        }
                }
        }

        return default_syscall_parser(buf, bufsize, syscall->args, offset, args);
}

static int default_syscall_parser(char *buf, size_t bufsize, 
        int nr_args, size_t *offset, raw_reg args[])
{
        for (int i = 0; i < nr_args; i++) {
                if (*offset >= bufsize)
                        break;

                const char *sep = (i == 0) ? "" : ", "; 

                *offset += snprintf(buf + (*offset), bufsize - (*offset), "%s%llu", sep, args[i]);
        }

        return 0;
}
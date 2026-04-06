#pragma once

#include <sys/types.h>

#include "parser/syscalls/parser.h"
#include "parser/syscalls/ABI/abi.h"

/*
 * FS_SYSCALL_LIST
 *
 * List of filesystem-related syscalls and their numbers.
 *
 * This macro is used as a single source of truth to:
 *  - declare parser functions
 *  - populate the parser table
 *
 * Each entry has the form:
 *      X(syscall_name, syscall_number)
 *
 * NOTE:
 *  - Commented entries indicate syscalls not yet implemented.
 */
#define FS_SYSCALL_LIST \
        X(read, 0) \
        X(write, 1) \
        // X(open, 2) \
        // X(close, 3) \


/*
 * Generate parser function declarations for each syscall.
 *
 * Expands to:
 *      int syscall_<name>_parser(...);
 */
#define X(name, _) DEFINE_SYSCALL_PARSER(name);
FS_SYSCALL_LIST
#undef X


/*
 * fs_syscalls
 *
 * Table of filesystem syscall parsers.
 *
 * Each entry maps:
 *  - syscall number (nr)
 *  - parser function responsible for formatting its arguments
 *
 * This table is later used by the central dispatcher to route
 * syscalls to the appropriate parser.
 */
const struct parser_struct fs_syscalls[] = {
#define X(name, nr) { nr, SYSCALL_PARSER_NAME(name) },
FS_SYSCALL_LIST
#undef X
};
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
        X(open, 2) \
        X(openat, 257) \
        X(close, 3) \
        X(rename, 82) \
        X(renameat, 264) \
        X(renameat2, 316) \
        X(unlink, 87) \
        X(unlinkat, 263) \
        X(link, 86) \
        X(linkat, 265) \
        X(symlink, 88) \
        X(symlinkat, 266) \
        X(lseek, 8) \
        X(pread64, 17) \
        X(pwrite64, 18) \
        X(readlink, 89) \
        X(readlinkat, 267) \
        X(getcwd, 79) \
        X(chdir, 80) \
        X(mkdir, 83) \
        X(mkdirat, 258) \
        X(rmdir, 84) \
        X(creat, 85) \
        X(truncate, 76) \
        X(ftruncate, 77) \
        X(stat, 4) \
        X(fstat, 5) \
        X(lstat, 6) \
        X(access, 21) \
        X(chmod, 90) \
        X(fchmod, 91) \
        X(chown, 92)

#define FS_SYSCALL_COUNT 34

/*
 * Generate parser function declarations for each syscall.
 *
 * Expands to:
 *      int syscall_<name>_parser(...);
 */
#define X(name, _) DEFINE_SYSCALL_PARSER(name);
FS_SYSCALL_LIST
#undef X

#define NAME_MAX 255

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
extern const struct parser_struct fs_syscalls[FS_SYSCALL_COUNT];

extern const char *STDFD_NAMES[3];
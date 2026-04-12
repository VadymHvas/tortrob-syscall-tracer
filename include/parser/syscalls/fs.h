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
 *       X(syscall_name, syscall_number, has_enter, has_exit)
 * where:
 * syscall_name   - name of the syscall
 * syscall_number - syscall number (architecture-specific)
 * has_enter      - one if Enter parser should be used
 * has_exit       - one if EXIT parser should be used
 *
 * NOTE:
 *  - Commented entries indicate syscalls not yet implemented.
 */
#define FS_SYSCALL_LIST \
        X(read,        0,   1, 1) \
        X(write,       1,   1, 0) \
        X(open,        2,   1, 0) \
        X(openat,      257, 1, 0) \
        X(close,       3,   1, 0) \
        X(rename,      82,  1, 0) \
        X(renameat,    264, 1, 0) \
        X(renameat2,   316, 1, 0) \
        X(unlink,      87,  1, 0) \
        X(unlinkat,    263, 1, 0) \
        X(link,        86,  1, 0) \
        X(linkat,      265, 1, 0) \
        X(symlink,     88,  1, 0) \
        X(symlinkat,   266, 1, 0) \
        X(lseek,       8,   1, 0) \
        X(pread64,     17,  1, 0) \
        X(pwrite64,    18,  1, 0) \
        X(readlink,    89,  1, 0) \
        X(readlinkat,  267, 1, 0) \
        X(getcwd,      79,  1, 0) \
        X(chdir,       80,  1, 0) \
        X(mkdir,       83,  1, 0) \
        X(mkdirat,     258, 1, 0) \
        X(rmdir,       84,  1, 0) \
        X(creat,       85,  1, 0) \
        X(truncate,    76,  1, 0) \
        X(ftruncate,   77,  1, 0) \
        X(stat,        4,   1, 1) \
        X(fstat,       5,   1, 1) \
        X(lstat,       6,   1, 1) \
        X(access,      21,  1, 0) \
        X(chmod,       90,  1, 0) \
        X(fchmod,      91,  1, 0) \
        X(chown,       92,  1, 0) \
        X(fchownat,    260, 1, 0) \
        X(statx,       332, 1, 1) \
        X(statfs,      137, 1, 0) \
        X(fstatfs,     138, 1, 0) \
        X(mknod,       133, 1, 0) \
        X(mknodat,     259, 1, 0) \
        X(fsync,       74,  1, 0) \
        X(fdatasync,   75,  1, 0) \
        X(dup,         32,  1, 0) \
        X(dup2,        33,  1, 0) \
        X(dup3,        292, 1, 0) \
        X(fcntl,       72,  1, 0) \
        X(readv,       19,  1, 1) \
        X(writev,      20,  1, 0) \
        X(preadv,      295, 1, 1) \
        X(pwritev,     296, 1, 0) \
        X(preadv2,     327, 1, 1) \
        X(pwritev2,    328, 1, 0) \
        X(umask,       95,  1, 0) \
        X(syncfs,      306, 1, 0) \
        X(fallocate,   285, 1, 0) \
        X(flock,       73,  1, 0) \
        X(fadvise64,   221, 1, 0) \
        X(newfstatat,  262, 1, 1)

#define FS_SYSCALL_COUNT 58

#define NAME_MAX 255

#define X(name, nr, has_enter, has_exit) \
        DEFINE_SYSCALL_ENTER_PARSER(name);

FS_SYSCALL_LIST
#undef X

#define X(name, nr, has_enter, has_exit) \
        DEFINE_SYSCALL_EXIT_PARSER(name);

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
extern const struct parser_struct fs_syscalls[FS_SYSCALL_COUNT];

extern const char *STDFD_NAMES[3];
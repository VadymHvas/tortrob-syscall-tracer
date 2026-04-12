#define _GNU_SOURCE

#include <sys/file.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"
#include "parser/syscalls/args/fs/value.h"
#include "parser/syscalls/args/fs/special.h"
#include "parser/syscalls/args/fs/flags.h"

#define INIT_VALUE(value) { value, #value }

DEFINE_VALUES_ARRAY(fcntl_cmds) = {
        INIT_VALUE(F_SETFD),
        INIT_VALUE(F_SETFL),
        INIT_VALUE(F_SETLK),
        INIT_VALUE(F_SETLKW),
        INIT_VALUE(F_SETOWN),
        INIT_VALUE(F_SETSIG),
        INIT_VALUE(F_SETPIPE_SZ),
        INIT_VALUE(F_DUPFD),
        INIT_VALUE(F_DUPFD_CLOEXEC),
        INIT_VALUE(F_GETFD),
        INIT_VALUE(F_GETFL),
        INIT_VALUE(F_GETOWN),
        INIT_VALUE(F_GETSIG),
        INIT_VALUE(F_GETPIPE_SZ),
        INIT_VALUE(F_GETLK)
};

DEFINE_VALUES_ARRAY(flock_ops) = {
        INIT_VALUE(LOCK_SH),
        INIT_VALUE(LOCK_EX),
        INIT_VALUE(LOCK_UN),
        INIT_VALUE(LOCK_NB),
};

DEFINE_VALUES_ARRAY(fadvise64_advices) = {
        INIT_VALUE(POSIX_FADV_NORMAL),
        INIT_VALUE(POSIX_FADV_SEQUENTIAL),
        INIT_VALUE(POSIX_FADV_RANDOM),
        INIT_VALUE(POSIX_FADV_NOREUSE),
        INIT_VALUE(POSIX_FADV_WILLNEED),
        INIT_VALUE(POSIX_FADV_DONTNEED),
};

DEFINE_VALUES_ARRAY(fs_types) = {
        { 0xef53,     "EXT2/EXT3/EXT4" },
        { 0x9123683e, "BTRFS" },
        { 0xf2f52010, "F2FS" },
        { 0x65735546, "FUSE" },
        { 0x9660,     "ISO9660" },
        { 0x3153464a, "JFS" },
        { 0x6969,     "NFS" },
        { 0x5346544e, "NTFS" },
        { 0x794c7630, "OVERLAYFS" },
        { 0x50495045, "PIPEFS" },
        { 0x9fa0,     "PROC" },
        { 0x62656572, "SYSFS" },
        { 0x01021994, "TMPFS" },
        { 0x74726163, "TRACEFS" },
        { 0x6e736673, "NSFS" },
        { 0x534f434b, "SOCKFS" },
        { 0x19800202, "MQUEUE" },
        { 0x15013346, "UDF" },
        { 0x58465342, "XFS" },
        { 0x01021997, "9P" },
        { 0x858458f6, "RAMFS" },
        { 0x9fa1,     "OPENPROM" },
        { 0xde5e81e4, "EFIVARFS" },
        { 0xcafe4a11, "BPF_FS" },
};

/* 
 * For F_GET* commands (except F_GETLK) we don't format 3rd argument, 
 * therefore ctx->extra is equals to FCNTL_IGNORE_ARG3.
 */ 
DEFINE_FMT(fcntl_cmd, int cmd)
{
        if (cmd == F_SETFL)
                ctx->extra = FCNTL_FLAGS;
        else if (cmd == F_SETFD)
                ctx->extra = FCNTL_FD_FLAGS;
        else if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK)
                ctx->extra = FCNTL_FLOCK;
        else if (cmd == F_SETOWN || cmd == F_SETPIPE_SZ || cmd == F_SETSIG)
                ctx->extra = FCNTL_INT;
        else if (cmd == F_GETFD || cmd == F_GETFL ||
                 cmd == F_GETOWN || cmd == F_GETPIPE_SZ ||
                 cmd == F_GETSIG)
                ctx->extra = FCNTL_IGNORE_ARG3;

        FOR_EACH_VALUES(fcntl_cmds) {
                if (cmd == fcntl_cmds[i].value) {
                        FMT_STRING(ctx, fcntl_cmds[i].name);

                        return 0;
                }
        }

        FMT_INT(ctx, cmd);

        ctx->extra = FCNTL_INVALID;
        return 0;
}

DEFINE_FMT(flock_op, int op)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, op);

        FOR_EACH_VALUES(flock_ops) {
                if (op == flock_ops[i].value) {
                        FMT_STRING(ctx, flock_ops[i].name);
                        return 0;
                }
        }

        FMT_INT(ctx, op);
        return 0;
}

DEFINE_FMT(fadvise64_advice, int advice)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, advice);

        FOR_EACH_VALUES(fadvise64_advices) {
                if (advice == fadvise64_advices[i].value) {
                        FMT_STRING(ctx, fadvise64_advices[i].name);
                        return 0;
                }
        }

        FMT_INT(ctx, advice);
        return 0;
}

DEFINE_FMT(statfs_fs_type, int fs_type)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, fs_type);

        FOR_EACH_VALUES(fs_types) {
                if (fs_type == fs_types[i].value) {
                        FMT_STRING(ctx, fs_types[i].name);
                        return 0;
                }
        }

        FMT_HEX(ctx, fs_type);

        return 0;
}
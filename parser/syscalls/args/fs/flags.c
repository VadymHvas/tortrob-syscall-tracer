#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"
#include "parser/syscalls/args/fs/flags.h"
#include "parser/syscalls/args/fs/mask.h"

DEFINE_FLAGS_ARRAY(open_flags) = {
        INIT_FLAG_INFO(O_CREAT),
        INIT_FLAG_INFO(O_EXCL),
        INIT_FLAG_INFO(O_TRUNC),
        INIT_FLAG_INFO(O_APPEND), 
        INIT_FLAG_INFO(O_NONBLOCK), 
        INIT_FLAG_INFO(O_DSYNC),
        INIT_FLAG_INFO(O_SYNC), 
        INIT_FLAG_INFO(O_RSYNC), 
        INIT_FLAG_INFO(O_CLOEXEC), 
        INIT_FLAG_INFO(O_DIRECTORY), 
        INIT_FLAG_INFO(O_NOFOLLOW),
        INIT_FLAG_INFO(O_TMPFILE)
};

DEFINE_FLAGS_ARRAY(open_modes) = {
        INIT_FLAG_INFO(O_RDONLY),
        INIT_FLAG_INFO(O_WRONLY),
        INIT_FLAG_INFO(O_RDWR)
};

DEFINE_FLAGS_ARRAY(at_flags) = {
        INIT_FLAG_INFO(AT_SYMLINK_FOLLOW),
        INIT_FLAG_INFO(AT_SYMLINK_NOFOLLOW),
        INIT_FLAG_INFO(AT_REMOVEDIR),
        INIT_FLAG_INFO(AT_NO_AUTOMOUNT),
        INIT_FLAG_INFO(AT_EMPTY_PATH)
};

DEFINE_FLAGS_ARRAY(statx_sync_modes) = {
        INIT_FLAG_INFO(AT_STATX_SYNC_AS_STAT),
        INIT_FLAG_INFO(AT_STATX_FORCE_SYNC),
        INIT_FLAG_INFO(AT_STATX_DONT_SYNC)
};

DEFINE_FLAGS_ARRAY(renameat2_flags) = {
        INIT_FLAG_INFO(RENAME_NOREPLACE),
        INIT_FLAG_INFO(RENAME_EXCHANGE),
        INIT_FLAG_INFO(RENAME_WHITEOUT)
};

DEFINE_FLAGS_ARRAY(rwf_flags) = {
        INIT_FLAG_INFO(RWF_DSYNC),
        INIT_FLAG_INFO(RWF_HIPRI),
        INIT_FLAG_INFO(RWF_SYNC),
        INIT_FLAG_INFO(RWF_NOWAIT),
        INIT_FLAG_INFO(RWF_APPEND)
};

/*
 * TODO: add unrecognized bits formatting
 */

DEFINE_FLAGS_FMT(open)
{
        FOR_EACH_FLAGS(open_modes) {
                if ((flags & O_ACCMODE) == open_modes[i].flag) {
                        FMT_STRING(ctx, open_modes[i].name);
                        break;
                }
        }

        FOR_EACH_FLAGS(open_flags) {
                if (flags & open_flags[i].flag) {
                        FMT_STRING(ctx, "|");
                        FMT_STRING(ctx, open_flags[i].name);
                }
        }

        return 0;
}

DEFINE_FLAGS_FMT(at)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        int first = 1;

        if (flags & AT_STATX_SYNC_TYPE) {
                int sync = flags & AT_STATX_SYNC_TYPE;

                FOR_EACH_FLAGS(statx_sync_modes) {
                        if (sync == statx_sync_modes[i].flag) {
                                FMT_FLAG_SEPARATOR(ctx, first);        
                                FMT_STRING(ctx, statx_sync_modes[i].name);
                                first = 0;
                                break;
                        }

                        flags &= ~AT_STATX_SYNC_TYPE;
                }
        }

        FOR_EACH_FLAGS(at_flags) {
                if (flags & at_flags[i].flag) {
                        FMT_FLAG_SEPARATOR(ctx, first);        
                        FMT_STRING(ctx, at_flags[i].name);
                        first = 0;
                }
        }
        
        return 0;
}

DEFINE_FLAGS_FMT(renameat2)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        int first = 1;

        FOR_EACH_FLAGS(renameat2_flags) {
                if (flags & renameat2_flags[i].flag) {
                        FMT_FLAG_SEPARATOR(ctx, first); 
                        FMT_STRING(ctx, renameat2_flags[i].name);
                        first = 0;
                }
        }

        return 0;
}

DEFINE_FLAGS_FMT(dup3)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        if (flags == O_CLOEXEC)
                FMT_STRING(ctx, "O_CLOEXEC");
        else
                FMT_INT(ctx, flags);

        return 0;
}

DEFINE_FLAGS_FMT(rwf)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        int first = 1;

        FOR_EACH_FLAGS(rwf_flags) {
                if (flags & rwf_flags[i].flag) {
                        FMT_FLAG_SEPARATOR(ctx, first);
                        FMT_STRING(ctx, rwf_flags[i].name);
                        first = 0;
                }
        }

        return 0;
}
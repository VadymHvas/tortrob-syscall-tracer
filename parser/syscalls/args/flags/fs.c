#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flags/fs.h"

#define FOR_EACH_FLAGS(flags_arr) for (int i = 0; i < sizeof(flags_arr) / sizeof(flags_arr[0]); i++)

static const struct flag_info open_flags[] = {
        { O_CREAT,     "O_CREAT" },
        { O_EXCL,      "O_EXECL" },
        { O_TRUNC,     "O_TRUNC" },
        { O_APPEND,    "O_APPEND" },
        { O_NONBLOCK,  "O_NONBLOCK" },
        { O_DSYNC,     "O_DSYNC" },
        { O_SYNC,      "O_SYNC" },
        { O_RSYNC,     "O_RSYNC" },
        { O_CLOEXEC,   "O_CLOEXEC" },
        { O_DIRECTORY, "O_DIRECTORY" },
        { O_NOFOLLOW,  "O_NOFOLLOW" },
        { O_TMPFILE,   "O_TMPFILE" }
};

static const struct flag_info open_modes[] = {
        { O_RDONLY, "O_RDONLY" },
        { O_WRONLY, "O_WRONLY" },
        { O_RDWR,   "O_RDWR" }
};

static const struct flag_info at_flags[] = {
        { AT_SYMLINK_FOLLOW,   "AT_SYMLINK_FOLLOW" },
        { AT_SYMLINK_NOFOLLOW, "AT_SYMLINK_NOFOLLOW" },
        { AT_REMOVEDIR,        "AT_REMOVEDIR" },
        { AT_NO_AUTOMOUNT,     "AT_NO_AUTOMOUNT" },
        { AT_EMPTY_PATH,       "AT_EMPTY_PATH" },
};

static const struct flag_info statx_sync_modes[] = {
        { AT_STATX_SYNC_AS_STAT, "AT_STATX_SYNC_AS_STAT" },
        { AT_STATX_FORCE_SYNC, "AT_STATX_FORCE_SYNC" },
        { AT_STATX_DONT_SYNC, "AT_STATX_DONT_SYNC" },
};

static const struct flag_info statx_masks[] = {
        { STATX_TYPE,   "STATX_TYPE" },
        { STATX_MODE,   "STATX_MODE" },
        { STATX_NLINK,  "STATX_NLINK" },
        { STATX_UID,    "STATX_UID" },
        { STATX_GID,    "STATX_GID" },
        { STATX_ATIME,  "STATX_ATIME" },
        { STATX_MTIME,  "STATX_MTIME" },
        { STATX_CTIME,  "STATX_CTIME" },
        { STATX_INO,    "STATX_INO" },
        { STATX_SIZE,   "STATX_SIZE" },
        { STATX_BLOCKS, "STATX_BLOCKS" },
        { STATX_BTIME,  "STATX_BTIME" },
        { STATX_ALL,    "STATX_ALL" }
};

static const struct flag_info renameat2_flags[] = {
        { RENAME_NOREPLACE, "RENAME_NOREPLACE" },
        { RENAME_EXCHANGE,  "RENAME_EXCHANGE" },
        { RENAME_WHITEOUT,  "RENAME_WHITEOUT" }
};

int fmt_open_flags(struct parser_ctx_struct *ctx, int flags)
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

int fmt_at_flags(struct parser_ctx_struct *ctx, int flags)
{
        if (!flags) {
                FMT_INT(ctx, 0);
                return 0;
        }

        int first = 1;

        if (flags & AT_STATX_SYNC_TYPE) {
                int sync = flags & AT_STATX_SYNC_TYPE;

                FOR_EACH_FLAGS(statx_sync_modes) {
                        if (sync == statx_sync_modes[i].flag) {
                                if (!first)
                                        FMT_STRING(ctx, "|");
                                        
                                FMT_STRING(ctx, statx_sync_modes[i].name);
                                first = 0;
                                break;
                        }

                        flags &= ~AT_STATX_SYNC_TYPE;
                }
        }

        FOR_EACH_FLAGS(at_flags) {
                if (flags & at_flags[i].flag) {
                        if (!first)
                                FMT_STRING(ctx, "|");

                        FMT_STRING(ctx, at_flags[i].name);
                        first = 0;
                }
        }
        
        return 0;
}

int fmt_renameat2_flags(struct parser_ctx_struct *ctx, int flags)
{
        if (!flags) {
                FMT_INT(ctx, 0);
                return 0;
        }

        int first = 1;

        FOR_EACH_FLAGS(renameat2_flags) {
                if (flags & renameat2_flags[i].flag) {
                        if (!first)
                                FMT_STRING(ctx, "|");

                        FMT_STRING(ctx, renameat2_flags[i].name);
                        first = 0;
                }
        }

        return 0;
}

int fmt_statx_mask(struct parser_ctx_struct *ctx, unsigned int mask)
{
        if (!mask) {
                FMT_INT(ctx, 0);
                return 0;
        }

        int first = 1;

        if ((mask & STATX_BASIC_STATS) == STATX_BASIC_STATS) {
                FMT_STRING(ctx, "STATX_BASIC_STATS");

                mask &= ~STATX_BASIC_STATS;
                first = 0;
        }

        FOR_EACH_FLAGS(statx_masks) {
                if (mask & statx_masks[i].flag) {
                        if (!first)
                                FMT_STRING(ctx, "|");
                                
                        FMT_STRING(ctx, statx_masks[i].name);

                        mask &= ~statx_masks[i].flag;
                        first = 0;
                }
        }

        return 0;
}
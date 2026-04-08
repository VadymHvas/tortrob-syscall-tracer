#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>

#include "parser/syscalls/helpers.h"
#include "parser/syscalls/flags.h"

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

static const struct flag_info statx_flags[] = {
        { AT_STATX_SYNC_AS_STAT, "AT_STATX_SYNC_AS_STAT" },
        { AT_STATX_FORCE_SYNC, "AT_STATX_FORCE_SYNC" },
        { AT_STATX_DONT_SYNC, "AT_STATX_DONT_SYNC" },
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
        if (flags == 0) {
                FMT_INT(ctx, 0);
                return 0;
        }

        int first = 1;

        FOR_EACH_FLAGS(at_flags) {
                if (flags & at_flags[i].flag) {
                        if (!first)
                                FMT_STRING(ctx, "|");

                        FMT_STRING(ctx, at_flags[i].name);
                        first = 0;
                }
        }

        if (flags & AT_STATX_SYNC_TYPE) {
                FOR_EACH_FLAGS(statx_flags) {
                        if (flags & statx_flags[i].flag) {
                                if (!first)
                                        FMT_STRING(ctx, "|");

                                FMT_STRING(ctx, statx_flags[i].name);
                                first = 0;
                        }
                }
        }

        return 0;
}

int fmt_renameat2_flags(struct parser_ctx_struct *ctx, int flags)
{
        if (flags == 0) {
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
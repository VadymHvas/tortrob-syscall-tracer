#define _GNU_SOURCE

#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"
#include "parser/syscalls/args/fs/flags.h"
#include "parser/syscalls/args/fs/mask.h"

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

        FOR_EACH_MASKS(statx_masks) {
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